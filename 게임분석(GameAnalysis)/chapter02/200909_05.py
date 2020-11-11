student = ['이황', '이이', '원효']
name = ''
print("전학 온 학생은 누구 입니까? : ")
name = input("")

student.append(name)
print(student)
print("번호 재정렬..")

student.sort()

for i in range(len(student)):
    print(i + 1, " ",  student[i])