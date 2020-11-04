class Employ:
    def __init__(self, name, year):
        self.name = name
        self.year = year

        print('{0}님의 연차는 {1}입니다.'.format(self.name, self.year))

    def salary(self):
        if(self.year <= 5):
            self.grade = "사원"
            self.salary_d = 3000 + (self.year * 100)
        elif(5 < self.year <= 10):
            self.grade = "대리"
            self.salary_d = 3500 + (self.year * 110)
        elif (self.year > 10):
            self.grade = "과장"
            self.salary_d = 4000 + (self.year * 130)

        print('{0}님의 직급은 {1}이며 연봉은 {2}만원입니다.'.format(self.name, self.grade, self.salary_d))

    def degree(self):
        self.salary_d += 1200
        print('{0}님의 연봉은 학위 소지로 인하여 {1}만원입니다.'.format(self.name, self.salary_d))

    def __getattr__(self, item):
        print('잘못된 값 입력입니다.')

a = Employ('김웅원', 7)
a.salary()
a.degree()
a.money

print(a.name, a.salary_d)
