# 메뉴 만들기

from tkinter import *

root = Tk()
root.title("Making Menu")

def nothing():
    print('Nothing_to_do')
def quit():
    root.destroy()
    root.quit()

menubar = Menu(root)
f1 = Menu(menubar, tearoff=0) #절취선 없애기
f1.add_command(label="새파일", command=nothing)
f1.add_command(label="열기", command=nothing)
f1.add_command(label="저장", command=nothing)
f1.add_separator()
f1.add_command(label="Exit", command=quit)
menubar.add_cascade(label="File", menu=f1) # f1메뉴 "File"이름으로 저장

f2 = Menu(menubar, tearoff=0)
f2.add_command(label="편집", command=nothing)
f2.add_command(label="삭제", command=nothing)
f2.add_command(label="복사", command=nothing)
menubar.add_cascade(label="편집", menu=f2)

root.config(menu=menubar) #메뉴바를 root창에 등록
root.mainloop()