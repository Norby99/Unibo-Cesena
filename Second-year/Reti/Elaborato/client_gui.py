import tkinter as tk
import json
from client import Client

class Client_GUI():
    """
    Class that implements a GUI for the Client class
    """

    __time_after: int = 100

    def __init__(self, ip, port):
        # create the main window
        self.root = tk.Tk()
        self.root.title("Client")
        self.root.geometry("300x300")
        self.root.resizable(False, False)

        self.client = Client(ip, port)
        self.create_widgets()
        
        self.root.after(self.__time_after, self.task)
        self.root.mainloop()
        
        self.client.close_connection()
        

    # create widgets for drone_id and shipment
    def create_widgets(self):
        # show available drones
        self.drones_list = tk.Listbox(self.root)
        self.drones_list.grid(row=0, column=0, columnspan=2)
        self.drones_list.insert(tk.END, "Available drones:")
        self.update()

        # ask for drone_id and shipment address
        self.drone_id_label = tk.Label(self.root, text="Drone ID:")
        self.drone_id_label.grid(row=1, column=0)
        self.drone_id_entry = tk.Entry(self.root)
        self.drone_id_entry.grid(row=1, column=1)
        self.shipment_label = tk.Label(self.root, text="Shipment address:")
        self.shipment_label.grid(row=2, column=0)
        self.shipment_entry = tk.Entry(self.root)
        self.shipment_entry.grid(row=2, column=1)
        self.submit_button = tk.Button(self.root, text="Submit", command=self.submit)
        self.submit_button.grid(row=3, column=0, columnspan=2)
        self.update_button = tk.Button(self.root, text="Update", command=self.update)
        self.update_button.grid(row=4, column=0, columnspan=2)
        self.quit_button = tk.Button(self.root, text="Quit", command=self.root.destroy)
        self.quit_button.grid(row=5, column=0, columnspan=2)



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

    def task(self) -> None:
        """
        Main task of the client.
        """
        self.client.main_tick()
        self.root.after(self.__time_after, self.task)

    


    

if __name__ == "__main__":
    with open('setup.json') as json_file:
        data = json.load(json_file)
        ip = data['gateway']["ip"]
        port = int(data['gateway']["port"])
        client = Client_GUI(ip, port)

