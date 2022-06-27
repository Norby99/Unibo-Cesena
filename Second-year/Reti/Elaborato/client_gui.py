import tkinter as tk
import json
from client import Client

class Client_GUI():
    """
    Class that implements a GUI for the Client class
    """

    __time_after: int = 100
    __time_update: int = 1000

    def __init__(self, ip, port):
        # create the main window
        self.root = tk.Tk()
        self.root.title("Client")
        self.root.geometry("300x300")
        self.root.resizable(False, False)

        self.client = Client(ip, port)
        self.create_widgets()
        
        self.root.after(self.__time_after, self.task)
        self.root.after(self.__time_after, self.update_task)
        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)
        self.root.mainloop()
        
        self.client.close_connection()
        

    # create widgets for drone_id and shipment
    def create_widgets(self):
        # show available drones on the left with no grid
        self.drones_list = tk.Listbox(self.root, width=20)
        self.drones_list.insert(tk.END, "")
        self.drones_list.pack(side=tk.TOP, fill=tk.X)

        # ask for drone_id and shipment address on the right
        self.drone_id_label = tk.Label(self.root, text="Drone ID:")
        self.drone_id_entry = tk.Entry(self.root, width=20)
        self.shipment_label = tk.Label(self.root, text="Shipment:")
        self.shipment_entry = tk.Entry(self.root, width=20)
        self.submit_button = tk.Button(self.root, text="Submit", command=self.submit)
        self.drone_id_label.pack(side=tk.TOP)
        self.drone_id_entry.pack(side=tk.TOP)
        self.shipment_label.pack(side=tk.TOP)
        self.shipment_entry.pack(side=tk.TOP)
        self.submit_button.pack(side=tk.TOP)

    def update(self):
        self.client.request_update()
        self.drones_list.delete(0, tk.END)
        for drone in self.client.get_free_drones():
            self.drones_list.insert(tk.END, drone)

    def submit(self):
        drone_id = self.drone_id_entry.get()
        shipment = self.shipment_entry.get()
        self.client.request_shipment(drone_id, shipment)
        self.drone_id_entry.delete(0, tk.END)
        self.shipment_entry.delete(0, tk.END)

    def update_task(self) -> None:
        """
        Update task of the client.
        """
        self.update()
        self.root.after(self.__time_update, self.update_task)

    def task(self) -> None:
        """
        Main task of the client.
        """
        self.client.main_tick()
        self.root.after(self.__time_after, self.task)

    def on_closing(self):
        self.client.close_connection()
        self.root.destroy()
        self.root.quit()

if __name__ == "__main__":
    with open('setup.json') as json_file:
        data = json.load(json_file)
        ip = data['gateway']["ip"]
        port = int(data['gateway']["port"])
        client = Client_GUI(ip, port)

