import socket
from threading import *
import os.path
import sys
import json
# I must do in this way to use it both as a library and a separate program
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.dirname(SCRIPT_DIR))
from libraries.udp_server import UDPServer


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
        self.socket_lock = Lock()
        self.__max_drone_limit = drone_limit
        self.__socket = super().configure_server()

    def _handle_request(self, data: str, drone_address) -> None:
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

        print(
            f'MESSAGE from {self.get_drone_id_by_address(drone_address)} : {drone_address}: ' + str(resp))

    def _send_request(self, request: str, drone_address) -> bool:
        """
        Send a requesto to a drone
        return False if something went wrong
        """
        # if the drone exist
        if not self.drone_exist(drone_address):
            print("The drone does not exist")
            return False

        # if the drone is free
        if not self.is_drone_free(self.get_drone_id_by_address(drone_address)):
            False

        # send response to the drone
        with self.socket_lock:
            self.__socket.sendto(request.encode('utf-8'), drone_address)

        self.set_drone_status(drone_address, "shipping")

    def wait_for_client(self) -> bool:
        '''
        Wait for clients and handle their requests
        return True if it's all ok
        '''
        try:
            try:  # receive request from client
                self.thread_request_handle()

            except OSError as err:
                print(err)

        except KeyboardInterrupt:
            self.shutdown_server()

        return True

    def thread_send_message(self, message, address) -> None:
        """
        Handles the send message with threads
        """
        send_req_thread = Thread(target=self._send_request,
                                           args=(message, address))
        send_req_thread.daemon = True
        send_req_thread.start()

    def thread_request_handle(self) -> None:
        """
        Handles the request with threads
        """
        request, address = self.__socket.recvfrom(
            self.__buffer_size)

        if not request:
            return

        if not self.drone_exist(address):
            if len(self.__drones) >= self.__max_drone_limit:
                print("Max drone limit")
                return False
            drone = self.create_drone(address)
            self.__drones[drone['id']] = drone
        
        if len(self.__drones) < self.__max_drone_limit:
            # listens for new clients in case the limit is not reached
            temp_thread = Thread(target=self.thread_request_handle)
            temp_thread.daemon = True
            temp_thread.start()

        req_thread = Thread(target=self._handle_request,
                                      args=(request, address))
        req_thread.daemon = True
        req_thread.start()

    def is_drone_free(self, drone_id: int) -> bool:
        """
        Return True if the given drone is free
        """
        for key, val in self.__drones.items():
            if val['id'] == drone_id:
                return True

        return False

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

    def get_drone_address_by_id(self, id) -> str:
        """
        Returns the drone address by the given id
        """
        for key, val in self.__drones.items():
            if val['id'] == id:
                return val['address']

    def get_drone_id_by_address(self, address) -> str:
        """
        Returns the id of the drone by the given address
        """
        for key, val in self.__drones.items():
            if val['address'] == address:
                return val['id']

        return None

    def get_free_drones(self) -> list[dict]:
        """
        Returns a list of the free drones
        """
        drones = []
        for key, val in self.__drones.items():
            if val['status'] == 'free':
                drones.append(val)

        return drones


if __name__ == '__main__':
    with open('setup.json') as json_file:
        data = json.load(json_file)

        udp_server_multi_client = UDPServerMultiClient(
            "", int(data['gateway']["port"]), int(data['max_drones']))
        while True:
            udp_server_multi_client.wait_for_client()
