from tkinter import *
window = Tk()
window.title("Calculator")
window.geometry("300x300+300+300")

flag = True
numbers = '1234567890'
operators = '/*-+'

def Calculate():
    global flag
    flag = False
    result = eval(display.get())
    display.delete(0, "end")  # 내용 삭제
    display.insert(0, result)

def Clear():
    global flag
    flag = True
    display.delete(0, "end")

def ButtonEvent(char):
    if flag == False:
        Clear()
    display.insert("end", char)

def Keyevent(event):
    if event.char in numbers:
        ButtonEvent(event.char)
    elif event.char in operators:
        ButtonEvent(event.char)
    elif event.keysym == "Return":
        Calculate()
    elif event.keysym == "Escape":
        Clear()
    elif event.keysym == "BackSpace":
        display.delete(len(display.get()) - 1, "end")

# Main Source
display = Entry(window, width=40)
display.grid(row=0,columnspan=4, padx=3, pady=3)

# ======================================================================================= 1열
button = Button(window, text='7', width=8, height=3, command=lambda :ButtonEvent('7')).grid(row=1, column=0, padx=3, pady=3)
button = Button(window, text='8', width=8, height=3, command=lambda :ButtonEvent('8')).grid(row=1, column=1, padx=3, pady=3)
button = Button(window, text='9', width=8, height=3, command=lambda :ButtonEvent('9')).grid(row=1, column=2, padx=3, pady=3)
button = Button(window, text='+', width=8, height=3, command=lambda :ButtonEvent('+')).grid(row=1, column=3, padx=3, pady=3)
# ======================================================================================= 2열
button = Button(window, text='4', width=8, height=3, command=lambda :ButtonEvent('4')).grid(row=2, column=0, padx=3, pady=3)
button = Button(window, text='5', width=8, height=3, command=lambda :ButtonEvent('5')).grid(row=2, column=1, padx=3, pady=3)
button = Button(window, text='6', width=8, height=3, command=lambda :ButtonEvent('6')).grid(row=2, column=2, padx=3, pady=3)
button = Button(window, text='-', width=8, height=3, command=lambda :ButtonEvent('-')).grid(row=2, column=3, padx=3, pady=3)
# ======================================================================================= 3열
button = Button(window, text='1', width=8, height=3, command=lambda :ButtonEvent('1')).grid(row=3, column=0, padx=3, pady=3)
button = Button(window, text='2', width=8, height=3, command=lambda :ButtonEvent('2')).grid(row=3, column=1, padx=3, pady=3)
button = Button(window, text='3', width=8, height=3, command=lambda :ButtonEvent('3')).grid(row=3, column=2, padx=3, pady=3)
button = Button(window, text='x', width=8, height=3, command=lambda :ButtonEvent('*')).grid(row=3, column=3, padx=3, pady=3)
# ======================================================================================= 4열
button = Button(window, text='C', width=8, height=3, command=Clear).grid(row=4, column=0, padx=3, pady=3)
button = Button(window, text='0', width=8, height=3, command=lambda :ButtonEvent('0')).grid(row=4, column=1, padx=3, pady=3)
button = Button(window, text='=', width=8, height=3, command=Calculate).grid(row=4, column=2, padx=3, pady=3)
button = Button(window, text='/', width=8, height=3, command=lambda :ButtonEvent('/')).grid(row=4, column=3, padx=3, pady=3)

window.bind("<Key>", Keyevent)

window.mainloop()