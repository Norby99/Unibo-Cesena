import json
import socket

class Gateway():

    __adress: tuple[str, int]
    __client_socket: socket.socket
    __buffer_size:int = 1024

    def __init__(self, gateway_port: int) -> None:
        self._setup_gateway_client(gateway_port)

        self.__connect_client()

    def _setup_gateway_client(self, gateway_port: int) -> None:
        """
        Setup the connection between the gateway and the client.
        """
        self.__adress = ("", gateway_port)
        self.__client_socket = socket.socket(family=socket.AF_INET, type=socket.SOCK_STREAM)
        self.__client_socket.bind(self.__adress)

    def __connect_client(self) -> bool:
        try:
            self.__client_socket.listen()
            self.__client_socket.accept()
            return True
        except:
            return False

    def close_client_connection(self) -> None:
        """
        Close the connection between the gateway and the client.
        """
        self.__client_socket.close()


if __name__ == '__main__':
    with open('setup.json') as json_file:
        data = json.load(json_file)
        gateway = Gateway(int(data['gateway']["port"]))
        
        gateway.close_client_connection()
