import socket
import json
from datetime import datetime
import time
import random

class Drone:
    ''' A simple UDP Client '''

    __address: tuple[str, int]
    __buffer_size: int = 1024

    def __init__(self, host, port):
        self.__address = (host, port)
        self.sock = None    # Socket

    def configure_client(self):
        ''' Configure the client to use UDP protocol with IPv4 addressing '''

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        print('Socket created')

    def ship_order(self, order: str):
        """
        Ship an order
        """
        print("Order recieved: " + order)
        # waiting...
        time.sleep(random.randint(5, 10))


    def interact_with_server(self):
        ''' Send request to a UDP Server and receive reply from it. '''

        try:
            msg = ''
            while True:
                # send data to server
                print('Sending [free] message')
                msg += 'free'
                self.sock.sendto(msg.encode('utf-8'), self.__address)

                # receive data from server
                order, server_address = self.sock.recvfrom(self.__buffer_size)
                self.ship_order(order)
                msg = 'order delivered '

        except OSError as err:
            print(err)
            print("There is no available server")

        finally:
            self.sock.close()

if __name__ == '__main__':
    with open('setup.json') as json_file:
        data = json.load(json_file)

        udp_client = Drone(data['gateway']["ip"], int(data['gateway']["port"]))
        udp_client.configure_client()
        udp_client.interact_with_server()
