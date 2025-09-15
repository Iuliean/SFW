import socket
import threading
import time
def connect_to_server(address, port):
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(10)
        sock.connect((address, port))
        return True
    except TimeoutError as e:
        print(str(e))
        return False
    except Exception as e:
        print(str(e))
        return False

class TSocket:
    def __init__(self):
        print("Initializing TSocket script");

    def SetUp(self):
        print("Setting up ...")

    def TearDown(self):
        print("Tearing down ...")

    def ListenAccept(self, address, port):
        print("ListenAccept -> IN")
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
            sock.connect((address,port))
            sock.shutdown(socket.SHUT_RDWR)
            sock.close()
        except Exception as e:
            print(str(e))
        print("ListenAccept -> OUT")

    def PollConnect(self, address, port):
        print("PollConnect -> IN")
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect((address, port))
            sock.shutdown(socket.SHUT_RDWR)
            sock.close()
        except Exception as e:
            print(str(e))

        print("PollConnect -> OUT")

