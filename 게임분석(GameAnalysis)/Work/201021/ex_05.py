# 동계올림픽 클래스

class WinterIOC:
    ice_choice = ""
    apply_count = 0

    def ice_game(self, value):
        self.apply_count = value

print("=" * 55)
print("■ 동계올림픽 클래스를 생성하는 프로그램")
print("-" * 55)

print("1. 클래스 생성완료 : WinterIOC")
print("-" * 55)

print("2. 인스턴스 생성 : MyGame")
print("-" * 55)

myGame = WinterIOC()

print("3. 인스턴스의 속성 지정 : 종목과 출전선수 인원")
print("-" * 55)

myGame.ice_choice = "컬링"
myGame.apply_count = 0

print("4. 메소드를 호출하여 인스턴스 출력")
print("-" * 55)

myGame.ice_game(4)

print(">> 출전종목 : %s" % myGame.ice_choice)
print(">> 출전인원 : %d" % myGame.apply_count)

print("-" * 55)
print(">> 프로그램을 종료합니다.")
print("=" * 55)