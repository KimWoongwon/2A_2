from  ex_01 import *

def lotto_start():
    print('-' * 55)
    print('>> 로또복권 추천번호입니다.\n')
    lotto = getNumber()
    lotto.sort()
    print(lotto)

    print('\n')
    print(">> 당첨을 기원합니다.")

