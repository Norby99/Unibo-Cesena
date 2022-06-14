import json
import socket

class Gateway():

    __adress: tuple[str, int]
    __client_socket: socket.socket
    __buffer_size:int = 1024
    __drones: dict = {
        "drone_1": {
            "id": "drone_1",
            "ip": "xxx",
            "status": "free"
        },
        "drone_2": {
            "id": "drone_2",
            "ip": "xxx",
            "status": "free"
        },
        "drone_3": {
            "id": "drone_3",
            "ip": "xxx",
            "status": "shipping"
        }
    }

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

    def __connect_client(self) -> None:
        self.__client_socket.listen()
        conn, addr = self.__client_socket.accept()
        print(f"Connected to Client: {addr}")

        with conn:
            self.send_drone_information(conn)

    def send_drone_information(self, conn: socket.socket) -> None:
        """
        Send the drones id that are free to the client.
        """
        free_drones = [drone["id"] for drone in self.__drones.values() if drone['status'] == 'free']
        conn.send(str(free_drones).encode())

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
