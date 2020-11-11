from tkinter import *
import tkinter.messagebox
root = Tk()

# Method
def KeyEvent(event):
    tkinter.messagebox.showinfo("Input Key Print", "Input Key :" + chr(event.keycode))

# Main Source
root.geometry("300x120+300+300")
root.title("KeyBoard Event")
root.bind("<Key>", KeyEvent)

lbl = Label(root, text="[Push Your Keyboard]")
lbl.pack(expand=1, anchor=CENTER)

root.mainloop()