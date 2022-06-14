import json
import socket

class Client():

    __adress: tuple[str, int]
    __gateway_socket: socket.socket
    __buffer_size: int = 1024

    def __init__(self, gateway_ip: str, gateway_port: int) -> None:
        self._setup_client_gateway(gateway_ip, gateway_port)

        if not self.__connect():
            raise Exception("Gateway is not available")

    def _setup_client_gateway(self, gateway_ip: str, gateway_port: int) -> None:
        """
        Setup the connection between the client and the gateway.
        """
        self.__adress = (gateway_ip, gateway_port)
        self.__gateway_socket = socket.socket(family=socket.AF_INET, type=socket.SOCK_STREAM)

    def __connect(self) -> bool:
        try:
            self.__gateway_socket.connect(self.__adress)
            return True
        except:
            return False

    def send_message(self, message: str) -> None:
        """
        Send a message to the gateway.
        """
        self.__gateway_socket.send(message.encode())

    def close_connection(self) -> None:
        """
        Close the connection between the client and the gateway.
        """
        self.__gateway_socket.close()

if __name__ == '__main__':
    with open('setup.json') as json_file:
        data = json.load(json_file)
        client = Client(data['gateway']["ip"], int(data['gateway']["port"]))
        client.send_message("Hello World")
        
        client.close_connection()
