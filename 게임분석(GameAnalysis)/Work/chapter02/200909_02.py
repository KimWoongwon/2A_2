s = " "
print("문장을 입력해 주세요. 'q' 입력시 종료합니다.")
while s != 'q':
    s = input(" ")
    print(s[0])
    for i in range(len(s)):
        if s[i] == ' ':
            print(s[i + 1])
