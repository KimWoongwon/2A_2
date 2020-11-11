# 마우스 왼쪽 버튼 클릭 이벤트
from tkinter import *
import tkinter.messagebox

root = Tk()
# 이벤트 처리 메소드
def ClickImg(event):
    tkinter.messagebox.showinfo('마우스이벤트', '마우스 왼쪽 버튼을 클릭했습니다.')

# Main Source
root.geometry("400x400+300+300")
root.title("설상경기종목")

img = PhotoImage(file="image_gif/snow-01.gif")
lbl = Label(root, image=img)
lbl.bind("<Button>", ClickImg)
lbl.pack(expand=1, anchor=CENTER)

root.mainloop()