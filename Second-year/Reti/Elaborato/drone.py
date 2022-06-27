import socket
import json
import time
import random


class Drone:
    '''
    A simple UDP Client
    '''
    
    __address: tuple[str, int]
    __buffer_size: int = 1024
    __socket: socket.socket

    def __init__(self, host, port) -> None:
        """
        Initializing the class
        """
        self.__address = (host, port)

    def configure_client(self) -> None:
        '''
        Configure the client to use UDP protocol with IPv4 addressing
        '''
        self.__socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def ship_order(self, order: str) -> None:
        """
        Ship an order
        """
        print("Order recieved: " + order)
        # waiting...
        time.sleep(random.randint(5, 10))
        print("Order shiped!")

    def interact_with_server(self) -> None:
        '''
        Send request to a UDP Server and receive reply from it.
        '''
        try:
            msg = []
            while True:
                # send data to server
                msg.append('free')
                msg = str(msg)

                print(f'Sending {msg} message')
                self.__socket.sendto(msg.encode('utf-8'), self.__address)

                print("Waiting for orders")

                # receive data from server
                order, server_address = self.__socket.recvfrom(
                    self.__buffer_size)
                order = order.decode()
                self.ship_order(order)
                msg = ['order delivered -> ' + order]

        except OSError as err:
            print(err)
            print("There is no available server")

        finally:
            self.__socket.close()


if __name__ == '__main__':
    with open('setup.json') as json_file:
        data = json.load(json_file)

        udp_client = Drone(data['gateway']["ip"], int(data['gateway']["port"]))
        udp_client.configure_client()
        udp_client.interact_with_server()
