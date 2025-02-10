import socket

class TSocket:
    def __init__(self):
        print("Initializing TSocket script");
    
    def SetUp(self):
        print("Setting up ...")

    def TearDown(self):
        print("Tearing down ...")

    def ListenAccept(self):
        print("ListenAccept -> IN")
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
            sock.connect(("127.0.0.1",12345))
        except Exception as e:
            print(str(e))
        print("ListenAccept -> OUT")
