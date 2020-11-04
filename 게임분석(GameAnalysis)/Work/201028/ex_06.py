## 슈퍼 클래스 정의
class WinterIOC:
    ice_choice = ""
    apply_count = 0
    nationality_count = 0

    def PrintInfo(self):
        print(">> 경기 종목 : %s" % self.ice_choice)
        print(">> 출전 국가 : %d개국" % WinterIOC.nationality_count)

##
class SubIce(WinterIOC):
    def __init__(self):
        self.ice_choice = ""
        self.apply_count = 0
        WinterIOC.nationality_count += 1

myGame_01 = SubIce()
myGame_01.ice_choice = "컬링"
myGame_01.PrintInfo()

myGame_02 = SubIce()
myGame_02.ice_choice = "봅슬레이"
myGame_02.PrintInfo()
