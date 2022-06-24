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

    def __init__(self, host, port):
        super().__init__(host, port)
        self.socket_lock = threading.Lock()

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

        print(f'[ REQUEST from {drone_address} ]')
        print(resp)

        msg = "ricevuto"
        # send response to the client
        with self.socket_lock:
            self.__socket.sendto(msg.encode('utf-8'), drone_address)
        print('\n', resp, '\n')

    def wait_for_client(self):
        '''
        Wait for clients and handle their requests
        '''

        try:
            while True: # keep alive
                try: # receive request from client
                    data, drone_address = self.__socket.recvfrom(self.__buffer_size)

                    c_thread = threading.Thread(target = self.handle_request,
                                            args = (data, drone_address))
                    c_thread.daemon = True
                    c_thread.start()

                except OSError as err:
                    print(err)

        except KeyboardInterrupt:
            self.shutdown_server()

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

    def drone_exists(self, address) -> bool:
    def drone_exist(self, address) -> bool:
        """
        Return True if the drone already exists
        """
        for key, val in self.__drones.items():
            if val['address'] == address:
                return True

        return False

if __name__ == '__main__':
    with open('setup.json') as json_file:
        data = json.load(json_file)

        udp_server_multi_client = UDPServerMultiClient("", int(data['gateway']["port"]))
        udp_server_multi_client.configure_server()
        udp_server_multi_client.wait_for_client()