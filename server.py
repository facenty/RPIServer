import threading
import socketserver
import os, sys
import json


class ThreadedTCPRequestHandler(socketserver.BaseRequestHandler):
    def handle(self):
        clientTypeJSON = str(self.request.recv(1024), "ascii")
        print(clientTypeJSON)
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
    # Port 0 means to select an arbitrary unused port
    HOST, PORT = "0.0.0.0", 9999

    server = ThreadedTCPServer((HOST, PORT), ThreadedTCPRequestHandler)
    with server:
        ip, port = server.server_address

        # Start a thread with the server -- that thread will then start one
        # more thread for each request
        # server_thread = threading.Thread(target=server.serve_forever)
        # Exit the server thread when the main thread terminates
        # server_thread.daemon = True
        # server_thread.start()
        # server_thread.join()
        server.serve_forever()
        # print("Server loop running in thread:", server_thread.name)

        # server.shutdown()
