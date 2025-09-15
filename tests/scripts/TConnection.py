import socket
import time
import threading
class TConnection:
    def __init__(self, address, port):
        print("Initializing TConnection script")
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((address, port))

    def __del__(self):
        print("Deinitializing TConnection script")
        self.sock.shutdown(socket.SHUT_RDWR)
        self.sock.close()

    def SetUp(self):
        print("Setting up ...")
        print("Setting up ... DONE")


    def TearDown(self):
        print("Tearing down ...")
        print("Tearing down ...DONE")

    def Echo(self, byte_count = 100):
        bytes = self.sock.recv(byte_count)
        self.sock.sendall(bytes)
