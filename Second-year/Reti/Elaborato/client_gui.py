import tkinter as tk
import json
from client import Client

class Client_GUI(tk.Frame):
    """
    Class that implements a GUI for the Client class
    """
    def __init__(self, ip, port, master=None):
        super().__init__(master)

        self.client = Client(ip, port)
        self.create_widgets()
        
        while True:
            self.client.main_tick()
        
        self.client.close_connection()
        

    # create widgets for drone_id and shipment
    def create_widgets(self):
        # show available drones
        self.drones_label = tk.Label(self, text="Available Drones:")
        self.drones_label.grid(row=0, column=0)
        self.drones_list = tk.Listbox(self)
        self.drones_list.grid(row=1, column=0)
        self.update_button = tk.Button(self, text="Update", command=self.update)
        self.update_button.grid(row=2, column=0)

        # show drone_id and shipment
        self.drone_id_label = tk.Label(self, text="Drone ID:")
        self.drone_id_label.grid(row=0, column=1)
        self.drone_id_entry = tk.Entry(self)
        self.drone_id_entry.grid(row=1, column=1)
        self.shipment_label = tk.Label(self, text="Shipment:")
        self.shipment_label.grid(row=0, column=2)
        self.shipment_entry = tk.Entry(self)
        self.shipment_entry.grid(row=1, column=2)
        self.submit_button = tk.Button(self, text="Submit", command=self.submit)
        self.submit_button.grid(row=2, column=2)

        # update button
        self.update_button = tk.Button(self, text="Update", command=self.update)
        self.update_button.grid(row=2, column=0)

    def update(self):
        self.client.request_update()
        self.drones_list.delete(0, tk.END)
        print(self.client.get_free_drones())
        for drone in self.client.get_free_drones():
            self.drones_list.insert(tk.END, drone)

    def submit(self):
        drone_id = self.drone_id_entry.get()
        shipment = self.shipment_entry.get()
        self.client.request_shipment(drone_id, shipment)
        self.drone_id_entry.delete(0, tk.END)
        self.shipment_entry.delete(0, tk.END)

    

    


    

if __name__ == "__main__":
    with open('setup.json') as json_file:
        data = json.load(json_file)
        ip = data['gateway']["ip"]
        port = int(data['gateway']["port"])
        client = Client_GUI(ip, port)

