## 올림픽 경기관람 티켓 구입가능 여부 판별 프로그램 ##
print("=" * 55)
print("■ 올림픽 경기관람 티켓 구입가능 여부판별 프로그램")
print("-" * 55)
print(">> 이 프로그램은 다음과 같이 수행됩니다.")
print(" a. 경기 종목 : 컬링 예선경기")
print(" b. 티켓 요금 : 4 만원")
print(" c. 구입 요령 : 현금 또는 카드 결제")
print("-" * 55)
print(">> 프로그램 수행 방법")
print(" a. 현금 결제 : 1 을 입력")
print(" b. 카드 결제 : 2 을 입력")
print(" c. 1,2 이외의 숫자 : 처리할 수 없음")
print("=" * 55)
choicevalue = eval(input(">> 결제 방법 (1. 현금결제 / 2. 카드결제): "))
print("-" * 55)

if  choicevalue == 1 or choicevalue == 2:
    print(">> 입력한 숫자 %d 은(는) 유효한 숫자 입니다." % choicevalue)
    print(">> 평창 동계 올림픽 [컬링 예선경기] 티켓 구입가능")

else:
    print(">> 입력한 숫자 %d 은(는) 유효한지 않은 숫자 입니다." % choicevalue)
    print(">> 입력 숫자를 다시 확인해 주세요...")

print("-" * 55)
print("프로그램을 종료 합니다.")
print("=" * 55)