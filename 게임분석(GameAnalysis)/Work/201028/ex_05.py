## 정적 메소드 사용하기
class Calc:
    @staticmethod
    def add(a,b):
        print(a + b)

    @staticmethod
    def mul(a,b):
        print(a * b)

## 메인함수
Calc.add(10, 20)
Calc.mul(10, 20)

## 클래스 메소드 사용하기

class Person:
    count = 0 # 클래스 속성

    def __init__(self):
        Person.count += 1

    @classmethod
    def Print_Count(cls):
        print("{0}명 생성되었습니다.".format(cls.count))

james = Person()
maria = Person()

Person.Print_Count()