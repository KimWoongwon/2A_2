## 이미지 버튼으로 메시지 창을 보여주는 프로그램
from tkinter import *
import tkinter.messagebox
rootwin = Tk()

## 메서드 정의
def mySports():
    tkinter.messagebox.showinfo("동계올림픽", "컬링경기-은메달수상")

## 메인 함수
photo = PhotoImage(file = "image_gif/ice-01.gif")
btn = Button(rootwin, image = photo, command = mySports)

btn.pack()
rootwin.mainloop()