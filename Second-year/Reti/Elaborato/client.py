import json
import socket

class Client():

    __adress: tuple[str, int]
    __gateway_socket: socket.socket
    __buffer_size: int = 1024

    def __init__(self, gateway_ip: str, gateway_port: int) -> None:
        self._setup_client_gateway(gateway_ip, gateway_port)

        if not self.__connect_to_gateway():
            raise Exception("Gateway is not available")

    def _setup_client_gateway(self, gateway_ip: str, gateway_port: int) -> None:
        """
        Setup the connection between the client and the gateway.
        """
        self.__adress = (gateway_ip, gateway_port)
        self.__gateway_socket = socket.socket(family=socket.AF_INET, type=socket.SOCK_STREAM)

    def __connect_to_gateway(self) -> bool:
        """
        Connects to the gateway
        """
        try:
            self.__gateway_socket.connect(self.__adress)
            return True
        except:
            return False

    def listen_for_drones(self) -> None:
        """
        Listen if there are free drones.
        """
        try:
            while True:
                data = self.__gateway_socket.recv(self.__buffer_size)
                if not data:
                    break
                free_drones = eval(data.decode("utf-8"))

                if type(free_drones) != list:
                    print("Wrong drone data format")
                    continue

                print(f"Free Drones: {free_drones}")

                self.request_shipment(self.choose_drone(free_drones))
        except Exception as e:
            print(e)
            print("Gateway disconnected!")
            self.close_connection()

    def request_shipment(self, msg: dict) -> None:
        """
        send shipment request
        """
        msg = json.dumps(msg).encode()
        self.__gateway_socket.send(msg)

    def choose_drone(self, free_drones: list[str]) -> dict:
        """
        Ask the user to input the drone name and the destination address
        Returns (chosen drone, destination address)
        """
        chosen_drone = ""
        destination_address = ""
        update_mode = False
        msg = {}

        while chosen_drone not in free_drones:
            chosen_drone = input("Insert the drone nome (insert 'update' to update the drone list): ")
            if chosen_drone == "update":
                update_mode = True
                break

        if not update_mode:
            destination_address = input("Insert the destination address: ")

            msg["drone_id"] = chosen_drone
            msg["order_destination"] = destination_address
        else:
            msg["update"] = 'update'

        return msg

    def close_connection(self) -> None:
        """
        Close the connection between the client and the gateway.
        """
        self.__gateway_socket.close()

if __name__ == '__main__':
    with open('setup.json') as json_file:
        data = json.load(json_file)
        client = Client(data['gateway']["ip"], int(data['gateway']["port"]))
        client.listen_for_drones()

        client.close_connection()