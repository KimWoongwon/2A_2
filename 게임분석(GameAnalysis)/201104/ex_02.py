#클래스 정의
class WintorIOC:
    def curling(self):
        print(">> 결승 진출 ")

class SubIce(WintorIOC):
    def curling(self):
        super().curling()
        print(">> 은메달 수상")

## 메인 프로그램 소스

print("=" * 55)
print("■ 상속관계에서 오버로딩 프로그램 ")
print("-" * 55)

medal = SubIce()
medal.curling()

print("-" * 55)
print(">> 프로그램을 종료 합니다.")
print("=" * 55)