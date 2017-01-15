import time
import serial
import threading
import Tkinter as tk
import ttk
import tkMessageBox
class myThread(threading.Thread):
    #initialize class
    def __init__(self, name, ser):
        threading.Thread.__init_self(self)
        self.name = name
        self.ser = ser

    def run(self):
        #counter of the while loop
        self.update_count = 0
        while self.ser.isOpen():
            self.update_count +=1
            UpdateC.set(set.update_count)

            for request in self.request_dict:
                try:
                    self.ser.write(request)
                    time.sleep(0.1)
                except:
                    pass

    def mQuit():
        mExit = tkMessageBox.askyesno(title="Quit")
        if mExit > 0:
            ser.close()
            root.destroy()
            return

    def mSend(command):
        try:
            ser.write(command)
        except:
            print("could not send command. Port closed?")

    def mSave(decision):

root = tk.Tk()
root.geometry("600x500")
root.title("Microwave Generator Control")
