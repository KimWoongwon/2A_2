#클래스 변수
class WinterIOC:
    ice_choice = ""
    apply_count = 0
    nationality_count = 0

    def __init__(self):
        self.ice_choice = ""    #Instance
        WinterIOC.nationality_count += 1    #Class

    def PrintInfo(self):
        print(">> 경기종목 : %s" % self.ice_choice)
        print(">> 출전국가 : %d\n" % WinterIOC.nationality_count)


myGame_01 = WinterIOC()
myGame_01.ice_choice = "컬링"
myGame_01.PrintInfo()

myGame_02 = WinterIOC()
myGame_02.ice_choice = "피겨 스케이팅"
myGame_02.PrintInfo()

myGame_03 = WinterIOC()
myGame_03.ice_choice = "봅슬레이"
myGame_03.PrintInfo()