from ex_02 import  *

while True:
    choiceValue = input(">> 로또번호 추천을 시작하겠습니다. ?(Y/N) : ")
    if choiceValue == 'Y' or choiceValue == 'y':
        lotto_start()
    elif choiceValue == 'N' or choiceValue == 'n':
        break
    else:
        print('>> 알파벳 입력오류')
        continue

print('>> 프로그램을 종료합니다.')
