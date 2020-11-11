## Pack방식으로 간단한 Dialog 보여주는 프로그램
from tkinter import *
rootwin = Tk()

## Label 생성 : pack()
lbl = Label(rootwin, text= "ID", font= ("굴림", 15), bg= "yellow")
lbl.pack()

## TextBox 생성 : pack()
txt = Entry(rootwin)
txt.pack()

## Button 생성 : pack()
btn = Button(rootwin, text="로그인", fg="blue")
btn.pack()

rootwin.mainloop()
