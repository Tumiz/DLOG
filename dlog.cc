// Copyright (c) Tumiz.
// Distributed under the terms of the GPL-3.0 License.

#include "dlog.h"

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <thread>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
class WebSocketContext{
  public:
  boost::asio::io_context ioc;
  websocket::stream<boost::asio::ip::tcp::socket> ws;
  WebSocketContext():ws{ioc}{};
};
WebSocketClient::WebSocketClient(const std::string &address) {
  try {
    // Look up the domain name
    context = std::make_unique<WebSocketContext>();
    std::regex re(":");
    std::vector<std::string> ss(
        std::sregex_token_iterator(address.begin(), address.end(), re, -1),
        std::sregex_token_iterator());
    boost::asio::ip::tcp::resolver resolver{context->ioc};
    auto const results = resolver.resolve(ss[0], ss[1]);

    // Make the connection on the IP address we get from a lookup
    auto ep = boost::asio::connect(context->ws.next_layer(), results);
    // Set a decorator to change the User-Agent of the handshake
    context->ws.set_option(
        websocket::stream_base::decorator([](websocket::request_type &req) {
          req.set(http::field::user_agent,
                  std::string(BOOST_BEAST_VERSION_STRING) +
                      " websocket-client-coro");
        }));

    // Perform the websocket handshake
    context->ws.handshake(address, "/");
  } catch (std::exception const &e) {
    std::cerr << __FUNCTION__ << ' ' << e.what() << std::endl;
  }
}
WebSocketClient::~WebSocketClient() {
  try {
    // Close the WebSocket connection
    context->ws.close(websocket::close_code::normal);
  } catch (std::exception const &e) {
    std::cerr << __FUNCTION__ << ' ' << e.what() << std::endl;
  }
}
void WebSocketClient::send(const std::string &message) {
  try {
    context->ws.write(boost::asio::buffer(message));
  } catch (std::exception const &e) {
    std::cerr << __FUNCTION__ << ' ' << e.what() << std::endl;
  }
}

FileLogger &FileLogger::Instance(const char *k) {
  static FileLogger logger;
  if (logger.streams.find(k) == logger.streams.end()) {
    std::cout << "create file:" << k << std::endl;
    logger.current_stream = std::make_shared<std::ofstream>(k);
    logger.streams.insert(
        std::map<const char *, std::shared_ptr<std::ofstream>>::value_type(
            k, logger.current_stream));
  }
  return logger;
}
FileLogger &FileLogger::operator<<(double v) {
  *current_stream << std::chrono::steady_clock::now().time_since_epoch().count()
                  << ',' << v << std::endl;
  return *this;
}

ChartLogger &ChartLogger::Instance(const char *k) {
  static ChartLogger logger;
  logger.variable_name = k;
  return logger;
}
ChartLogger &ChartLogger::operator<<(double v) {
  std::ostringstream oss;
  oss << variable_name << ','
      << std::chrono::high_resolution_clock::now().time_since_epoch().count() << ',' << v
      << std::endl;
  ws.send(oss.str());
  return *this;
}

ChartLogger::ChartLogger() : ws("127.0.0.1:8000"){};