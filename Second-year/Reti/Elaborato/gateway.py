import json
import socket
from libraries.udp_server_multi_client import UDPServerMultiClient


class Gateway():

    __address: tuple[str, int]
    __client: dict = {
        "socket": socket.socket,
        "address": tuple[str, int],
        "conn": socket.socket,
        "is_connected": False
    }
    __buffer_size: int = 1024
    __drone_server: UDPServerMultiClient
    __is_client_connected: bool = False

    def __init__(self, gateway_ip: str, gateway_port: int, max_drones: int) -> None:
        self.__drone_server = UDPServerMultiClient(
            gateway_ip, gateway_port, max_drones)
        self._setup_gateway_client(gateway_ip, gateway_port)

    def _setup_gateway_client(self, gateway_ip: str, gateway_port: int) -> None:
        """
        Setup the connection between the gateway and the client.
        """
        self.__address = (gateway_ip, gateway_port)
        self.__client["socket"] = socket.socket(
            family=socket.AF_INET, type=socket.SOCK_STREAM)
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
            while True:
                self.send_drone_information()
                data = self.__client["conn"].recv(self.__buffer_size)
                if not data:
                    break
                order = eval(data.decode("utf-8"))
                if not self._check_order(order):
                    self._send_message_to_client("Error: wrong order syntax")
                elif "update" in order:
                    continue

                print(f"Order: {order}")
                self._process_order(order)
        except:
            print(f"Client {self.__client['address']} disconnected!")
            self.__is_client_connected = True

    def send_drone_information(self) -> bool:
        """
        Send the drones id that are free to the client.
        """
        free_drones = self.__drone_server.get_free_drones()
        try:
            self.__client["conn"].send(str(free_drones).encode())
            return True
        except:
            print(f"Client {self.__client['address']} disconnected!")
            self.__is_client_connected = False
            return False

    def _send_message_to_client(self, message: str) -> None:
        """
        Send a message to the client.
        """
        self.__client["conn"].send(message.encode())

    def _process_order(self, order: dict) -> None:
        """
        Process the order and send the drone to the order destination.
        """
        # TODO: the code below is wrong
        print(order)
        self.__drone_server.thread_send_message(order["order_destination"], )

    def _check_order(self, order: dict) -> bool:
        """
        Check if the order is valid and if the drone is available.
        """

        if "update" in order:
            return True

        # check if the order syntax is valid
        if not all(keys in order for keys in ("drone_id", "order_destination")):
            print("Wrong order syntax")
            return False

        # check if the drone exists
        if order["drone_id"] not in self.__drone_server.get_drones():
            print("The selected drone does not exist")
            return False

        # check if the drone is free
        if order["drone_id"] not in self.__drone_server.get_free_drones():
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
        gateway = Gateway("", int(data['gateway']["port"]), data["max_drones"])
        gateway.listen_for_orders()

        gateway.close_client_connection()
