## 올림픽 경기관람 티켓 구입가능 여부 판별 프로그램 ##
print("=" * 55)
print("■ 올림픽 경기관람 티켓 구입가능 여부판별 프로그램")
print("-" * 55)
cash = eval(input(">> 가지고 있는 현금 입력 : "))
print("-" * 55)

if cash >= 40000:
    print(">> 현금 " + format(cash, ',d')+" 원은 [컬링 예선경기] 티켓 구입가능")
    money = cash - 40000
    print(">> 현금 " + format(money, ',d')+" 원은 저축하세요...")

else:
    print(">> 현금 " + format(cash, ',d') + " 원은 현금 부족으로 티켓 구입불가")
    money = 40000 - cash
    print(">> 현금 " + format(money, ',d') + " 원이 더 필요합니다...")

print("-" * 55)
print("프로그램을 종료 합니다.")
print("=" * 55)