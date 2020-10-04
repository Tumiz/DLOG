// Copyright (c) Tumiz.
// Distributed under the terms of the GPL-3.0 License.

#ifndef DLOG_H
#define DLOG_H
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <memory>
class WebSocketContext;
class WebSocketClient {
 private:
  std::unique_ptr<WebSocketContext> context;
 public:
  WebSocketClient(const std::string& address);
  ~WebSocketClient();
  void send(const std::string& message);
};
class FileLogger {
 public:
  static FileLogger& Instance(const char* k);
  FileLogger& operator<<(double v);

 private:
  std::shared_ptr<std::ofstream> current_stream;
  std::map<const char*, std::shared_ptr<std::ofstream>> streams;
  FileLogger(){};
  ~FileLogger(){};
};
class ChartLogger{
  public:
  static ChartLogger& Instance(const char* k);
  ChartLogger& operator<<(double v);

 private:
  WebSocketClient ws;
  const char* variable_name;
  ChartLogger();
  ~ChartLogger(){};
};

FileLogger& File(const char* k){
  return FileLogger::Instance(k);
}

ChartLogger& Chart(const char* k){
  return ChartLogger::Instance(k);
}
#endif