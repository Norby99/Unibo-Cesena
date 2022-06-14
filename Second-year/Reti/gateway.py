import json
import socket

class Gateway():

    __address: tuple[str, int]
    __client: dict = {
        "socket": socket.socket,
        "address": tuple[str, int],
        "conn": socket.socket,
        "is_connected": False
    }
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
    __is_client_connected: bool = False

    def __init__(self, gateway_port: int) -> None:
        self._setup_gateway_client(gateway_port)

        self.__connect_client()

    def _setup_gateway_client(self, gateway_port: int) -> None:
        """
        Setup the connection between the gateway and the client.
        """
        self.__address = ("", gateway_port)
        self.__client["socket"] = socket.socket(family=socket.AF_INET, type=socket.SOCK_STREAM)
        self.__client["socket"].bind(self.__address)

    def __connect_client(self) -> None:
        self.__client["socket"].listen()
        self.__client["conn"], self.__client["address"] = self.__client["socket"].accept()
        if (not self.__is_client_connected):
            print(f"Connected to Client: {self.__client['address']}")
            self.__is_client_connected = True

        with self.__client["conn"]:
            self.send_drone_information()

    def send_drone_information(self) -> None:
        """
        Send the drones id that are free to the client.
        """
        free_drones = [drone["id"] for drone in self.__drones.values() if drone['status'] == 'free']
        try:
            self.__client["conn"].send(str(free_drones).encode())
        except:
            print(f"Client {self.__client['address']} disconnected!")
            self.__is_client_connected = False

    def close_client_connection(self) -> None:
        """
        Close the connection between the gateway and the client.
        """
        self.__client["socket"].close()


if __name__ == '__main__':
    with open('setup.json') as json_file:
        data = json.load(json_file)
        gateway = Gateway(int(data['gateway']["port"]))

        gateway.close_client_connection()
