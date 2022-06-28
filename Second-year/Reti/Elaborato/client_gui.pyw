import tkinter as tk
import json
from client import Client

class ClientGUI():
    """
    Class that implements a GUI for the Client class
    """

    __time_after: int = 100
    __time_update: int = 1000

    def __init__(self, ip, port) -> None:
        """
        Constructor of the Client_GUI class.
        """
        self.root = tk.Tk()
        self.root.title("Client")
        self.root.geometry("300x300")
        self.root.resizable(False, False)

        self.client = Client(ip, port)
        self.create_widgets()
        
        self.__time_after, self.task()
        self.update_task()

        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)
        self.root.mainloop()
        
        self.client.close_connection()
        
    def create_widgets(self) -> None:
        """
        Create the widgets of the GUI.
        """
        # show available drones on the left with no grid
        self.drones_list = tk.Listbox(self.root, width=20)
        self.drones_list.insert(tk.END, "")
        self.drones_list.pack(side=tk.TOP, fill=tk.X)
        self.drones_list.bind("<Double-Button-1>", self.drone_list_click)

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

        # show the status of the client
        self.status_text = tk.Label(self.root, text="")
        self.status_text.pack(side=tk.BOTTOM)

    def drone_list_click(self, event):
        """
        When the user clicks on a drone in the list,
        the drone_id entry is filled with the drone_id.
        """
        try:
            drone_id = self.drones_list.get(self.drones_list.curselection())
            self.drone_id_entry.delete(0, tk.END)
            self.drone_id_entry.insert(tk.END, drone_id)
        except:
            pass

    def update(self) -> None:
        """
        Update the list of drones.
        """
        self.client.request_update()
        self.drones_list.delete(0, tk.END)
        for drone in self.client.get_free_drones():
            self.drones_list.insert(tk.END, drone)

    def submit(self) -> None:
        """
        Submit the order.
        """
        msg = {}

        drone_id = self.drone_id_entry.get()
        shipment = self.shipment_entry.get()

        msg["drone_id"] = drone_id
        msg["order_destination"] = shipment

        if self.client.order_checker(msg):
            self.client.request_shipment(msg)
            self.status_text.config(text="")
        else:
            self.status_text.config(text="Invalid drone")

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
        self.client.main_tick_thread()
        self.root.after(self.__time_after, self.task)

    def on_closing(self):
        """
        Close the connection and close the window.
        """
        self.client.close_connection()
        self.root.destroy()
        self.root.quit()

if __name__ == "__main__":
    with open('setup.json') as json_file:
        data = json.load(json_file)
        ip = data['gateway']["ip"]
        port = int(data['gateway']["port"])
        client = ClientGUI(ip, port)

