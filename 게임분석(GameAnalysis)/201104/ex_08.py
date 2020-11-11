## 라디오 버튼과 레이블 위젯으로 4개 항목중 하나를 선택하는 프로그램
from tkinter import *
rootwin = Tk()

## 메서드 정의부분
def clickMe():
    if chk.get() == 1:
        lbl.configure(text="컬링")
    elif chk.get() == 2:
        lbl.configure(text="피겨 스케이팅")
    elif chk.get() == 3:
        lbl.configure(text="봅슬레이")
    else:
        lbl.configure(text="선택안함")

## 메인 함수
chk = IntVar()
rbn1 = Radiobutton(rootwin, text='컬링', variable= chk, value = 1, command = clickMe)
rbn2 = Radiobutton(rootwin, text='피겨 스케이팅', variable= chk, value = 2, command = clickMe)
rbn3 = Radiobutton(rootwin, text='봅슬레이', variable= chk, value = 3, command = clickMe)
rbn4 = Radiobutton(rootwin, text='선택안함', variable= chk, value = 4, command = clickMe)
lbl= Label(rootwin, text = '선택한 종목 : ', fg = "blue")

rbn1.pack()
rbn2.pack()
rbn3.pack()
rbn4.pack()
lbl.pack()

rootwin.mainloop()
