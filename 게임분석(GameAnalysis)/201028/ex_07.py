class Person:
    def Greeting(self):
        print("안녕하세요")


class Student(Person):
    def Study(self):
        print("공부")

james = Student()
james.Greeting()
james.Study()

##포함관계
class Person2:
    def Greeting(self):
        print("안녕하세요")

class PersonList:
    def __init__(self):
        self.person_list = []

    def append_person(self, person):
        self.person_list.append(person)
