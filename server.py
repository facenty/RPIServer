import socketserver
import os
import json


class ThreadedTCPRequestHandler(socketserver.BaseRequestHandler):
    def handle(self):
        clientTypeJSON = str(self.request.recv(1024), "ascii")
        # print(clientTypeJSON)
        clientTypeStr = json.loads(clientTypeJSON)["ClientType"]
        self.request.sendall(bytes("OK", "ASCII"))
        if clientTypeStr == "PUBLISHER":
            while True:
                data = self.request.recv(1024)
                if len(data) == 0:
                    print("Client disconnected")
                    return
                os.write(self.server.w, data)
        if clientTypeStr == "SUBSCRIBER":
            while True:
                response = os.read(self.server.r, 1024)
                try:
                    self.request.sendall(response)
                except:
                    return


class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    def __init__(self, server_address, RequestHandlerClass):
        socketserver.TCPServer.__init__(self, server_address, RequestHandlerClass)
        self.r, self.w = os.pipe()


if __name__ == "__main__":
    HOST, PORT = "0.0.0.0", 9999

    server = ThreadedTCPServer((HOST, PORT), ThreadedTCPRequestHandler)
    with server:
        ip, port = server.server_address
        server.serve_forever()
