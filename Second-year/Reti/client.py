import json
import socket

class Client():

    __adress: tuple[str, int]
    __gateway_socket: socket.socket
    __buffer_size:int = 1024

    def __init__(self, gateway_ip: str, gateway_port: int) -> None:
        self.__adress = (gateway_ip, gateway_port)
        self.__gateway_socket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

        if not self.__test_connection():
            raise Exception("Gateway is not available")

    def __test_connection(self) -> bool:
        self.__gateway_socket.sendto("test".encode(), self.__adress)
        try:
            response = self.__gateway_socket.recv(self.__buffer_size)
            return True
        except:
            return False

    def send_message(self, message: str) -> None:
        self.__gateway_socket.sendto(message.encode(), self.__adress)

if __name__ == '__main__':
    with open('setup.json') as json_file:
        data = json.load(json_file)
        client = Client(data['gateway']["ip"], int(data['gateway']["port"]))
        client.send_message("Hello World")
