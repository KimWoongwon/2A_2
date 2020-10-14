
## D타입의 사용자 정의 함수 소스코드
def d_type(choiceValue):
    print("-" * 38);
    print("\n>> 시작 : 사용자 정의 함수 - D타입\n");
    number = {
        1: '컬링',
        2: '피겨 스케이팅',
        3: '알파인 스키',
        4: '봅슬레이',
        5: '쇼트트랙',
        6: '그냥종료'
    }

    if choiceValue == 1:
        prt(choiceValue)
        return number.get(choiceValue)
    elif choiceValue == 2:
        prt(choiceValue)
        return number.get(choiceValue)
    elif choiceValue == 3:
        prt(choiceValue)
        return number.get(choiceValue)
    elif choiceValue == 4:
        prt(choiceValue)
        return number.get(choiceValue)
    elif choiceValue == 5:
        prt(choiceValue)
        return number.get(choiceValue)
    elif choiceValue == 6:
        prt(choiceValue)
        return number.get(choiceValue)
    else:
        print(">> 유효숫자 입력 오류!!\n")

## 중복되는 출력문을 함수로 정의
def prt(value):
    print(">> 메인에서 입력한 값 : %d\n" % value)
    print(">> 종료 : 사용자 정의 함수 - D타입\n")

## 메인 프로그램 소스 코드
print("=" * 55);
print('D타입의 사용자 정의 함수 프로그램')

print("-" * 55);
print('>> 선택할 종목은 다음과 같습니다.')

number = """
    1. 컬링
    2. 피겨 스케이팅
    3. 알파인 스키
    4. 봅슬레이
    5. 쇼트트랙
    6. 그냥 종료
"""

print(number);
print("-" * 55);

su = int(input(">> 선택 (1~6) : "))

print(">> 사용자 정의 함수를 호출합니다...")
value = d_type(su)
print(">> 종목 : [ %s ] \n" % value)
print("-" * 38);

print(">> 메인 프로그램을 종료합니다.")
print("=" * 55)
