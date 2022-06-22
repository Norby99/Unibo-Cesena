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

    def listen_for_orders(self) -> None:
        """
        Listen if there are orders to be shipped.
        """
        self.__connect_client()

        try:
            self.send_drone_information()

            while True:
                data = self.__client["conn"].recv(self.__buffer_size)
                if not data:
                    break
                order = eval(data.decode("utf-8"))
                if not self._check_order(order):
                    self._send_message("Error: wrong order syntax")

                print(f"Order: {order}")
                self._process_order(order)
        except:
            print(f"Client {self.__client['address']} disconnected!")
            self.__is_client_connected = True

    def send_drone_information(self) -> bool:
        """
        Send the drones id that are free to the client.
        """
        free_drones = [drone["id"] for drone in self.__drones.values() if drone['status'] == 'free']
        try:
            self.__client["conn"].send(str(free_drones).encode())
            return True
        except:
            print(f"Client {self.__client['address']} disconnected!")
            self.__is_client_connected = False
            return False

    def _send_message(self, message: str) -> None:
        """
        Send a message to the client.
        """
        self.__client["conn"].send(message.encode())

    def _process_order(self, order: dict) -> None:
        """
        Process the order and send the drone to the order destination.
        """
        self.__drones[order["drone_id"]]["status"] = "shipping"
        #TODO: send drone to order destination

    def _check_order(self, order: dict) -> bool:
        """
        Check if the order is valid and if the drone is available.
        """
        # check if the order syntax is valid
        if not all(keys in order for keys in ("drone_id", "order_destination")):
            print("Wrong order syntax")
            return False

        # check if the drone exists
        if order["drone_id"] not in self.__drones.keys():
            print("The selected drone does not exist")
            return False

        # check if the drone is free
        if self.__drones[order["drone_id"]]["status"] != "free":
            print("The selected drone is not free")
            return False
        
        return True

    def close_client_connection(self) -> None:
        """
        Close the connection between the gateway and the client.
        """
        self.__client["socket"].close()


if __name__ == '__main__':
    with open('setup.json') as json_file:
        data = json.load(json_file)
        gateway = Gateway(int(data['gateway']["port"]))
        gateway.listen_for_orders()

        gateway.close_client_connection()
