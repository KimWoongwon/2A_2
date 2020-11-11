# 마우스 왼쪽 버튼 클릭 이벤트
from tkinter import *
import tkinter.messagebox

root = Tk()

# 이벤트 처리 메소드
def ClickLeft(event):
    tkinter.messagebox.showinfo('마우스이벤트', '마우스 왼쪽 버튼을 클릭했습니다.')

# Main Source

root.bind("<Button-1>", ClickLeft)
root.mainloop()