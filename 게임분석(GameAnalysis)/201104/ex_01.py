#클래스 정의
class WintorIOC:
    def curling(self):
        print(">> 결승 진출 ")

class SubIce(WintorIOC):
    def curling(self):
        print(">> 은메달 수상")

## 메인 프로그램 소스

print("=" * 55)
print("■ 상속관계에서 오버로딩 프로그램 ")
print("-" * 55)

print(">> 슈퍼클래스 메서드 호출")

battle1 = WintorIOC()
battle1.curling()

print("-" * 55)
print(">> 서브클래스 메서드 호출")

battle2 = SubIce()
battle2.curling()

print("-" * 55)
print(">> 프로그램을 종료 합니다.")
print("=" * 55)