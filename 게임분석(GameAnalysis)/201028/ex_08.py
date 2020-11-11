## 기반클래스 속성 사용하기
class Person:
    def __init__(self):
        print('Person __init__')
        self.hello = '안녕'

class Student(Person):
    pass
    #def __init__(self):
        #print('Student __init__')
        #super().__init__()
        #self.school = '광주중앙고'


james = Student()
#print(james.school)
print(james.hello)