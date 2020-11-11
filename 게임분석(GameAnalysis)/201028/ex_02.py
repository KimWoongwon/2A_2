## 클래스 선언시 속성을 공개
class Person:
    def __init__(self, name, age, address, wallet):
        self.hello = '안녕하세요'
        self.name = name
        self.age = age
        self.address = address
        self.__wallet = wallet
    def pay(self, amount):
        self.__wallet -= amount
        print('이제 {0}원 남았습니다.'.format(self.__wallet))

##Main_Function
maria = Person('마리아', 20, '서울시 서초구 반포동', 10000)
maria.pay(3000)

