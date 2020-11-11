# 10개의 이미지를 고정 위치에 중복 for문을 이용해 배치하는 프로그램
from tkinter import *

# Variable Define
imgList = ["ice-01.gif", "ice-02.gif", "ice-03.gif", "ice-04.gif", "ice-05.gif",
           "snow-01.gif", "snow-02.gif", "snow-03.gif", "snow-04.gif", "snow-05.gif"]

btnList = [None] * 10

low, col = 0, 0
xPos, yPos = 0, 0
cnt = 0

# Main Source
rootwin = Tk()
rootwin.geometry("1230x480+300+300")
rootwin.title("동계올림픽 선호종목")

for low in range(0, 10):
    btnList[low] = PhotoImage(file="image_gif/" + imgList[low])
    imgList[low] = Button(rootwin, image=btnList[low])

for low in range(0, 2):
    for col in range(0, 5):
        imgList[cnt].place(x=xPos, y=yPos)
        cnt += 1
        xPos += 250
    xPos = 0
    yPos += 250

rootwin.mainloop()


