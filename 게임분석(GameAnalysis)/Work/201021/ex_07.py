class WinterIOC:
    ice_choice = ""
    apply_count = 0

    def __init__(self, ice, apply):
        self.ice_choice = ice
        self.apply_count = apply

    def get_ice(self):
        return self.ice_choice

    def get_apply(self):
        return self.apply_count

myGame_01 = WinterIOC("컬링", 4)
myGame_02 = WinterIOC("피겨스케이팅", 2)

print(myGame_01.get_ice(), myGame_01.get_apply())
print(myGame_02.get_ice(), myGame_02.get_apply())
