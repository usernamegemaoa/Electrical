from tkinter import *
from tkinter import Tk, Frame, BOTH

class Example(Frame):

    def __init__(self, parent):
        Frame.__init__(self, parent, background="white")

        self.parent = self
        self.initUI()

    def initUI(self):
        #self.parent.title("Simple")
        self.pack(fill=BOTH, expand=1)

    def counter_label(self, label):
        def count():
            global counter
            counter += 1
            label.config(text=str(counter))
            label.after(1000, count)
        count()

def main():
    root = Tk()
    root.title("Slithr: BLDC")
    root.geometry("1000x750")
    logo = PhotoImage(file="C:/Users/Intern-Carl/PycharmProjects/untitled/sw-caution.gif")
    w = Label(root, text="Slithr!", fg="red", bg="dark green", font="Lobster").pack()
    w1 = Label(root, image=logo).pack(side="right")
    text1 = "Slithr: Customizable Electric Skateboard"
    w2 = Label(root, justify=LEFT, padx=10, text=text1).pack(side="left")
    label1 = Label(root, fg="green").pack()
    app = Example(root)
    app.counter_label(label1)
    Tk.Button(root, fg="red", text='Stop!', width=25, command=root.destroy).pack()
    root.mainloop()

if __name__== '__main__':
    main()