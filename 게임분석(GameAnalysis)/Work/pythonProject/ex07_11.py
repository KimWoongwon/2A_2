## 딕셔너리, get함수 사용 ##
print("=" * 55)
print("■ 딕셔너리를 사용한 관람종목 판별 프로그램")
print("-" * 55)
print(">> 동계올림픽 관람종목은 다음과 같습니다.")
print(" 1. 컬링")
print(" 2. 피겨 스케이팅")
print(" 3. 알파인 스키")
print(" 4. 봅슬레이")
print(" 5. 쇼트트랙")
print("-" * 55)

number = eval(input(">> 관람종목 선택(1~5): "))
print("-" * 55)

if number >= 1 and number <= 5:
    choiceValue = {1: '컬링', 2:'피겨 스케이팅', 3:'알파인 스키', 4:'봅슬레이', 5:'쇼트트랙'}
    print(">> 선택한 종목 : ", end='')
    print(choiceValue.get(number))
else:
    print(">> 입력한 [%d] 은(는) 유효숫자가 아닙니다." % number)
    print(">> 프로그램을 다시 시작하세요.")

print("-" * 55)
print("프로그램을 종료 합니다.")
print("=" * 55)
