name = input('이름을 입력하세요 : ')
tall = int(input('키(cm)를 입력하세요 : '))
weight = int(input('몸무게(kg)를 입력하세요 : '))

height = tall / 100
BMI = weight / (height * height)
BMIstr = ""

if BMI >= 30:
    BMIstr = "고도비만"
elif 29.9 > BMI >= 25:
    BMIstr = "비만"
elif 24.9 > BMI >= 23:
    BMIstr = "과체중"
elif 22.9 > BMI >= 18.5:
    BMIstr = "정상"
else:
    BMIstr = "정상이하"

print(name, '님의 키는', tall, 'cm이고 몸무게는', weight, 'kg 입니다.')
print('BMI 지수는', format(BMI, '.2f'), '입니다. ', end='')
print(BMIstr, '입니다.')


