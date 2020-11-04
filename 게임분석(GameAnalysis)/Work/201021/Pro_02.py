## 계산기만들기
class Calculator:

    def plus(self, x, y):
        return x + y
    def minus(self, x, y):
        return x - y
    def multiply(self, x, y):
        return x * y
    def divide(self, x, y):
        return x / y

calc = Calculator()

print(">> 계산기 프로그램")
print(">> 2개의 숫자를 입력하세요")
a = int(input("첫번째 숫자 : "))
b = int(input("첫번째 숫자 : "))

print(calc.plus(a,b))
print(calc.minus(a,b))
print(calc.multiply(a,b))
print(calc.divide(a,b))