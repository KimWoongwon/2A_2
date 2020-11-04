## 체크박스로 메시지 창을 보여주는 프로그램
from tkinter import *
import tkinter.messagebox
rootwin = Tk()

## 메소드 정의
def clickMe():
    if chk.get() != 0:
        tkinter.messagebox.showinfo("선택여부", "컬링경기를 선택했습니다.")
    else:
        tkinter.messagebox.showinfo("선택여부", "아무것도 선택하지 않았습니다.")

## 메인 함수
chk = IntVar()
cbn = Checkbutton(rootwin, text="컬링경기", variable=chk, command= clickMe)
cbn.pack()
rootwin.mainloop()