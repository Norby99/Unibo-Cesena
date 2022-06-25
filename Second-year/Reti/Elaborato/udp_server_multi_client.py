import socket
import threading
from libraries.udp_server import UDPServer
import json


class UDPServerMultiClient(UDPServer):
    '''
    A simple UDP Server for handling multiple clients
    '''

    __buffer_size: int = 1024
    __socket: socket.socket
    __drones: dict = {}
    __max_drone_limit: int

    def __init__(self, host: str, port: int, drone_limit: int):
        super().__init__(host, port)
        self.socket_lock = threading.Lock()
        self.__max_drone_limit = drone_limit

    def configure_server(self):
        self.__socket = super().configure_server()

    def handle_request(self, data: str, drone_address):
        '''
        Handle the client
        '''
        # handle request
        resp = data.decode('utf-8')
        if self.valid_response(resp):
            resp = eval(resp)
        else:
            self.shutdown_server()

        if 'free' in resp:
            self.set_drone_status(drone_address, 'free')
        else:
            self.shutdown_server()

        print(f'MESSAGE from {drone_address}: ' + str(resp))

        msg = "via Roma 7"
        # send response to the client
        with self.socket_lock:
            self.__socket.sendto(msg.encode('utf-8'), drone_address)
        print(msg)

    def wait_for_client(self) -> bool:
        '''
        Wait for clients and handle their requests
        return True if it's all ok
        '''
        try:
            try:  # receive request from client
                data, drone_address = self.__socket.recvfrom(
                    self.__buffer_size)

                if not self.drone_exist(drone_address):
                    if len(self.__drones) >= self.__max_drone_limit:
                        print("Max drone limit")
                        return False
                    drone = self.create_drone(drone_address)
                    self.__drones[drone['id']] = drone

                c_thread = threading.Thread(target=self.handle_request,
                                            args=(data, drone_address))
                c_thread.daemon = True
                c_thread.start()

            except OSError as err:
                print(err)

        except KeyboardInterrupt:
            self.shutdown_server()

        return True

    def valid_response(self, msg) -> bool:
        """
        Check if the drone response is correct
        """
        try:
            msg = eval(msg)
            if isinstance(msg, list) or isinstance(msg, tuple):
                if len(msg) <= 2:
                    return True
        except:
            pass

        print("No valid Data format")
        return False

    def create_drone(self, address) -> dict:
        """
        Returns a drone dict
        """
        id = len(self.__drones)+1
        drone_id = "drone_" + str(id)
        return {
            "id": drone_id,
            "address": address,
            "status": "free"
        }

    def drone_exist(self, address) -> bool:
        """
        Return True if the drone already exists
        """
        for key, val in self.__drones.items():
            if val['address'] == address:
                return True

        return False

    def get_dron_id_by_address(self, address) -> str:
        """
        Returns the id of the drone by the given address
        """
        for key, val in self.__drones.items():
            if val['address'] == address:
                return val['id']

        return None

    def set_drone_status(self, address, status: str) -> bool:
        """
        Set free status to a given drone
        """
        for key, val in self.__drones.items():
            if val['address'] == address:
                self.__drones[key]['status'] = status
                return True

        print("Error drone not found")
        return False

    def get_drones(self) -> dict:
        """
        Returns the dict of the drones
        """
        return self.__drones


if __name__ == '__main__':
    with open('setup.json') as json_file:
        data = json.load(json_file)

        udp_server_multi_client = UDPServerMultiClient(
            "", int(data['gateway']["port"]), int(data['max_drones']))
        udp_server_multi_client.configure_server()
        while True:
            udp_server_multi_client.wait_for_client()
