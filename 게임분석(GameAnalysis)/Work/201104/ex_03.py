from tkinter import *
class Curling(Frame):
    def __init__(self, sports):
        img = PhotoImage(file = "image_gif/ice-01.gif")
        lbl = Label(image = img)
        lbl.image = img
        lbl.place(x = 0, y = 0)

##메인함수
def main():
    rootwin = Tk()
    rootwin.title("빙상경기")
    rootwin.geometry("230x230+20+20")
    rootwin.resizable(width=FALSE, height=FALSE)
    mylike = Curling(rootwin)
    rootwin.mainloop()

if __name__ == "__main__":
    main()