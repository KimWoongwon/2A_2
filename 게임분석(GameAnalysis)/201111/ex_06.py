# 마우스 왼쪽 버튼 클릭 이벤트
from tkinter import *

root = Tk()
# 이벤트 처리 메소드
def ClickMouse(event):
    txtpos = ""
    if event.num == 1:
        txtpos += "Mouse Left Click Position : ["
    elif event.num == 3:
        txtpos += "Mouse Right Click Position : ["

    txtpos += str(event.x) + " , " + str(event.y) + "]"
    lbl.configure(text=txtpos)

# Main Source
root.geometry("500x300+300+300")
root.title("Mouse Click Position Event")
lbl = Label(root, text = "[Mouse Position Print]")

root.bind("<Button>", ClickMouse)
lbl.pack(expand=1, anchor=S)

root.mainloop()
