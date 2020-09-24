student = []

dic = {'학번':'', '이름':'', '생년월일':'', '전화번호':'', '주소': ''}

number = int(input("학번 : "))
name = input("이름 : ")
birth = input("생년월일 : ")
phone = input("전화번호 : ")
address = input("주소 : ")

student.append({'학번':number, '이름':name, '생년월일':birth, '전화번호':phone, '주소': address})

print("학생의 정보 입력 완료")
print(student[0])
