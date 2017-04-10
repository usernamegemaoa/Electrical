import serial

import tkinter as tk
from tkinter import ttk

import matplotlib
matplotlib.use("TkAgg")
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2TkAgg
from matplotlib.figure import Figure
import matplotlib.animation as animation
from matplotlib import style
from matplotlib import pyplot as plt

import urllib
import json

import pandas as pd
import numpy as np


LARGE_FONT= ("Verdana", 12)
NORM_FONT = ("Helvetica", 10)
SMALL_FONT = ("Helvetica", 8)
style.use("ggplot")

f = Figure(figsize=(10,6), dpi=100)
a = f.add_subplot(111)

port1 = serial.Serial(port='COM3', baudrate=9600, bytesize=serial.EIGHTBITS, stopbits=serial.STOPBITS_ONE, timeout=None)

def popupmsg(msg):
    popup = tk.Tk()
    popup.wm_title("Pop-Up Message!")
    label = ttk.Label(popup, text=msg, font=NORM_FONT)
    label.pack(side="top", fill="x", pady=10)
    B1 = ttk.Button(popup, text="Okay", command = popup.destroy)
    B1.pack()
    popup.mainloop()

def animate(i):
    dataLink = 'https://btc-e.com/api/3/trades/btc_usd?limit=2000'
    data = urllib.request.urlopen(dataLink)
    data = data.readall().decode("utf-8")
    data = json.loads(data)

    data = data["btc_usd"]
    data = pd.DataFrame(data)

    buys = data[(data['type'] == "bid")]
    buys["datestamp"] = np.array(buys["timestamp"]).astype("datetime64[s]")
    buyDates = (buys["datestamp"]).tolist()

    sells = data[(data['type'] == "ask")]
    sells["datestamp"] = np.array(sells["timestamp"]).astype("datetime64[s]")
    sellDates = (sells["datestamp"]).tolist()

    a.clear()

    a.plot_date(buyDates, buys["price"], "#00A3E0", label="buys")
    a.plot_date(sellDates, sells["price"], "#183A54", label="sells")

    a.legend(bbox_to_anchor=(0, 1.02, 1, .102), loc=3,
             ncol=2, borderaxespad=0)

    title = "BTC-e BTCUSD Prices\nLast Price: " + str(data["price"][1999])
    a.set_title(title)

def print_Name1():
        print("Carl!")

class SensorGlove(tk.Tk):
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)

        tk.Tk.iconbitmap(self, default="sw-caution.gif")
        tk.Tk.wm_title(self, "Sensor Glove Gui")

        container = tk.Frame(self)
        container.pack(side="top", fill="both", expand=True)
        container.grid_rowconfigure(0, weight=1)
        container.grid_columnconfigure(0, weight=1)

        menubar = tk.Menu(container)

        fileMenu = tk.Menu(menubar, tearoff=0)
        fileMenu.add_command(label="Save settings", command=lambda: popupmsg("Not supported just yet!"))
        fileMenu.add_separator()
        fileMenu.add_command(label="Exit", command=quit)
        menubar.add_cascade(label="File", menu=fileMenu)

        subMenu = tk.Menu(menubar, tearoff=1)
        subMenu.add_command(label="duh...", command=lambda: print_Name1())
        subMenu.add_command(label="Save settings", command=lambda: popupmsg("Not supported just yet!"))
        subMenu.add_command(label="Now...", command= lambda: print_Name1())
        subMenu.add_separator()
        subMenu.add_command(label="Exit", command=lambda: print_Name1())
        menubar.add_cascade(label="Sub", menu=subMenu)

        editMenu = tk.Menu(menubar, tearoff=1)
        editMenu.add_command(label="Redo", command=lambda: print_Name1())
        editMenu.add_separator()
        editMenu.add_command(label="Exit", command=lambda: print_Name1())
        menubar.add_cascade(label="Edit", menu=editMenu)

        #status = tk.Label(container, text="Waiting for command...", bd=1)

        tk.Tk.config(self, menu=menubar)

        self.frames = {}

        for F in (Second_Page, Main_Page):

            frame = F(container, self)

            self.frames[F] = frame

            frame.grid(row=0, column=0, sticky="nsew")

        self.show_frame(Main_Page)
    def show_frame(self, cont):

        frame = self.frames[cont]
        frame.tkraise()


class Second_Page(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self,parent)
        label = tk.Label(self, text=("Welcome to Sensor Glove Gui!"), font=LARGE_FONT)
        label.pack(pady=10,padx=10)

        button1 = ttk.Button(self, text="Start!",
                            command=lambda: controller.show_frame(Main_Page))
        button1.pack()

        button2 = ttk.Button(self, text="Quit",
                            command=quit)
        button2.pack()

class Main_Page(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)

        sp1 = Serial_Port()


        title_Commands = tk.Label(self, text="Sensor-Glove Gui", font=LARGE_FONT)
        title_Commands.pack(anchor="n", side=tk.LEFT, pady=10)

        button1 = ttk.Button(self, text="Get Device ID:", command=lambda: sp1.print_ID())
        button1.pack(anchor="n", side=tk.LEFT)

        s_toggle = [0] * 8

        S1_check = tk.Checkbutton(self, text="Sensor 1", onvalue = 1, offvalue = 0, command=lambda: update_CheckBox_Sensors(self,1))
        S1_check.pack()

        S2_check = tk.Checkbutton(self, text="Sensor 2", onvalue = 1, offvalue = 0, command =lambda: update_CheckBox_Sensors(self,2))
        S2_check.pack()

        S3_check = tk.Checkbutton(self, text="Sensor 3", onvalue = 1, offvalue = 0, command =lambda: update_CheckBox_Sensors(self,3))
        S3_check.pack()

        S4_check = tk.Checkbutton(self, text="Sensor 4", onvalue = 1, offvalue = 0, command =lambda: update_CheckBox_Sensors(self,4))
        S4_check.pack()

        S5_check = tk.Checkbutton(self, text="Sensor 5", onvalue = 1, offvalue = 0, command=lambda: update_CheckBox_Sensors(self,5))
        S5_check.pack()

        S6_check = tk.Checkbutton(self, text="Sensor 6", onvalue = 1, offvalue = 0, command =lambda: update_CheckBox_Sensors(self,6))
        S6_check.pack()

        S7_check = tk.Checkbutton(self, text="Sensor 7", onvalue = 1, offvalue = 0, command =lambda: update_CheckBox_Sensors(self,7))
        S7_check.pack()

        S8_check = tk.Checkbutton(self, text="Sensor 8", onvalue = 1, offvalue = 0,variable=s_toggle[7], command =lambda : update_CheckBox_Sensors(self))
        S8_check.pack()

        canvas = FigureCanvasTkAgg(f, self)
        canvas.show()
        canvas.get_tk_widget().pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)

        toolbar = NavigationToolbar2TkAgg(canvas, self)
        toolbar.update()
        canvas._tkcanvas.pack(side=tk.TOP, fill=tk.BOTH, expand=True)

        def update_CheckBox_Sensors(self):
            #sp1.update_Sensor(8, int(s_toggle[b-1]))
            print(s_toggle)

class Serial_Port:
    def __init__(self):
        self.data = []

    def read_8(self):
        return port1.read()

    def write_8(self, data):
        message = data.encode('utf-8')
        port1.write(message)

    def read_dec(self):
        temp_a = self.read_8()
        temp_b = self.read_8()
        temp_c = (temp_b[0] << 8) | temp_a[0]
        return temp_c

    def print_Sensor(self):
        sensor = self.read_dec()
        print('Sensor Value: {:f}'.format(sensor))

    def read_hex(self):
        temp_a = hex(self.read_dec())
        return temp_a

    def print_ID(self, event):
        self.write_8('i')
        deviceID = self.read_hex()
        print('Device ID: ' + repr(deviceID))

    def print_ID(self):
        self.write_8('i')
        deviceID = self.read_hex()
        print('Device ID: ' + repr(deviceID))

    def print_Voltage(self):
        analog = self.read_dec()
        voltage = (analog/1024 * 5)
        print('Voltage: {:f}'.format(voltage))

    def print_Name(self):
        print("Huggie!")

    def update_Sensor(self, g, t):
        print(str(t))


def main():
    sp1 = Serial_Port()
    while 1:
        try:
            sp1.write_8('s')
            indicator = sp1.read_8()
            if indicator == b'v':
                sp1.print_Voltage()

            if indicator == b'i':
                sp1.print_ID()

            if indicator == b's':
                sp1.print_Sensor()

        except port1.SerialTimeoutException:
            print('Data could not be read')
            time.sleep(1)

#    app = SensorGlove()
#    app.geometry("1280x720")
#    ani = animation.FuncAnimation(f, animate, interval=1000)
#    app.mainloop()


if __name__ == "__main__":
    main()

