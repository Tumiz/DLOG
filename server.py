# Copyright (c) Tumiz.
# Distributed under the terms of the GPL-3.0 License.

import os
import json
import time
import tornado.web
import tornado.websocket

class IndexHandler(tornado.web.RequestHandler):
    def get(self,v):
        print(__name__,v)
        self.render("dlog.html")

class WSHandler(tornado.websocket.WebSocketHandler):
    nodes=set()
    viewers=set()
    def open(self,v):
        print("ws open",v,self.request)
        if v=="viewer":
            WSHandler.viewers.add(self)
            self.type="viewer"
        else:
            WSHandler.nodes.add(self)
            self.type="node"
        
    def on_message(self, message):
        for viewer in WSHandler.viewers:
            viewer.write_message(message)
        
    def on_close(self):
        if self.type == "viewer":
            WSHandler.viewers.remove(self)
        else:
            WSHandler.nodes.remove(self)
        print("ws close",self.request,self.close_code,self.close_reason)

    
    def check_origin(self, origin):
        return True

static_path=os.path.join(os.path.dirname(__file__), "static")
template_path=os.path.join(os.path.dirname(__file__), "template")
app = tornado.web.Application([
        (r"/view/(.*)", IndexHandler),
        (r"/(.*)", WSHandler),
        (r"/(.*)",tornado.web.StaticFileHandler)      
    ],
    static_path = static_path,
    template_path = template_path,
    debug = True
)
http_server = tornado.httpserver.HTTPServer(app)
http_server.listen(8000)
loop=tornado.ioloop.IOLoop.current()
loop.start() 
