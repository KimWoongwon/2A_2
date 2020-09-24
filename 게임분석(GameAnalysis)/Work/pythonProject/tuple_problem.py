## 튜플조작기 ##
print("■ 튜플조작기 ver1")
str = input("튜플에 입력할 자료들을 차례로 입력하세요. 빈칸으로 구분합니다.\n")
str = tuple(str.split(' '))

count = 0
while 1:
    print("작업할내용을 입력하세요.")
    choice = input(" q. 끝내기 / s. 슬라이싱 / c. 세기 / i = 존재여부 : ")

    if choice == 's':
        f = int(input("From : "))
        t = int(input("To : "))
        print(str[f:t])
    elif choice == 'c':
        f = input("찾을 자료 값은 : ")
        print(str.count(f), " 번 있습니다." )
    elif choice == 'i':
        f = input("찾을 자료 값은 : ")
        print(f in str)
    elif choice == 'q':
        print("프로그램을 종료합니다.")
        break
    else:
        print("잘못된 입력값입니다.")

