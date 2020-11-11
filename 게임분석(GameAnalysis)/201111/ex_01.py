from tkinter import *

rootwin = Tk()
rootwin.geometry("200x50+300+300")

# Create Button
btn1 = Button(rootwin, text="Yes", fg="blue")
btn2 = Button(rootwin, text="No", fg="red")
btn3 = Button(rootwin, text="Cancel", bg="yellow")

btn1.pack(side=LEFT, padx=10, pady=10)
btn2.pack(side=LEFT, padx=10, pady=10)
btn3.pack(side=LEFT, padx=10, pady=10)

rootwin.mainloop()
