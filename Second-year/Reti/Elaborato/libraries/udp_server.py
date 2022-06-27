import socket
from abc import ABC, abstractmethod


class UDPServer(ABC):
    ''' A simple UDP Server '''

    __address: tuple[str, int]
    __socket: socket.socket

    def __init__(self, host, port):
        self.__address = (host, port)

    def configure_server(self) -> socket.socket:
        '''
        Configure the server
        '''
        print('Creating socket...')
        self.__socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        print(f'Binding server to {self.__address}...')
        self.__socket.bind(self.__address)
        return self.__socket

    @abstractmethod
    def _handle_request(self, data, client_address):
        '''
        Handle the client
        '''
        return

    @abstractmethod
    def wait_for_client(self):
        '''
        Wait for a client
        '''
        return

    def shutdown_server(self):
        ''' Shutdown the UDP server '''
        self.__socket.close()
