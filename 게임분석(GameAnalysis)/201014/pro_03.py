#도서 대출 시스템
#'홍길동' '홍길은' '홍길금'

bookList = {
    1 : '도레미제라블',
    2 : '위대한 소츠비',
    3 : '어른왕자',
    4 : '아이와 산',
    5 : '곤충 농장',
    6 : '춘추전국시대',
    7 : '분노의 사과',
    8 : '강철북',
    9 : '첫번째 잎새',
    10 : '그리고 아무말이나 했다'
}
borrower = {}
UserList = ['홍길동','홍길은', '홍길금' ]

def borrow():
    borrowList = []
    for i in range(0,3):
        index = int(input('빌릴 책의 코드 : '))
        borrowList.append(bookList[index])
    return borrowList


while True:
    a = input("회원 이름을 입력하세요 : ")
    if a == 'q':
        break

    flag = False
    for i in UserList:
        if a == i:
            flag = True
    if flag:
        borrower[a] = borrow()
    else:
        print("없는 회원정보 입니다.\n")

print("현재 대출자 목록")
print(borrower)