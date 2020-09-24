## 리스트와 문자열로 변경 ##
print("=" * 55)
print("■ 리스트와 문자열로 티켓 구입가능 여부판별 프로그램")
print("-" * 55)
print(">> 결제방법은 다음과 같이 수행됩니다.")
print(" a. 현금 결제 : cash")
print(" b. 카드 결제 : card")
print(" c. 핸드폰 결제 : phone")
print("=" * 55)

ticket = ['cash', 'card', 'phone']
print(">> 문자열 입력 예시 --> space")
choicevalue = input(">> 결제 방법 문자열로 입력하시오: ")
print("-" * 55)

if choicevalue in ticket:
    print(">> [%s] 결제 방법을 선택하였습니다." % choicevalue)
    print(">> 동계 올림픽 티켓 구입가능")

else:
    print(">> 입력한 [%s] 은(는) 유효하지 않은 문자열 입니다." % choicevalue)
    print(">> 문자열을 다시 확인해 주세요...")

print("-" * 55)
print("프로그램을 종료 합니다.")
print("=" * 55)
