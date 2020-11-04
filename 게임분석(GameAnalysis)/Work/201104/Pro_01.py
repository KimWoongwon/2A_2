## 로그인창 생성하기
from tkinter import *
import tkinter.messagebox
rootwin = Tk()
rootwin.resizable(False, False)

## 메서드 정의
def Login():
    if id.get() == '':
        tkinter.messagebox.showinfo("로그인 실패", "아이디를 입력해주세요.")
    else:
        if id.get() in UserInfo:
            if UserInfo[id.get()] == pw.get():
                tkinter.messagebox.showinfo("로그인 성공", "%s님 안녕하세요" % id.get())
            else:
                if pw.get() == '':
                    tkinter.messagebox.showinfo("로그인 실패", "비밀번호를 입력해주세요.")
                else:
                    tkinter.messagebox.showinfo("로그인 실패", "비밀번호가 틀렸습니다.")
        else:
            tkinter.messagebox.showinfo("로그인 실패", "없는 아이디 입니다.")



## 메인 함수

UserInfo = {"김웅원":"abc123"}

id_lbl = Label(rootwin, text='ID를 입력하세요', fg="black")
id = Entry(rootwin)
pw_lbl = Label(rootwin, text='PW를 입력하세요', fg="black")
pw = Entry(rootwin, show="●")

btn = Button(rootwin, text='로그인',width = 10, command=Login)

id_lbl.pack()
id.pack()
pw_lbl.pack()
pw.pack()
btn.pack()

type(UserInfo)
rootwin.mainloop()

