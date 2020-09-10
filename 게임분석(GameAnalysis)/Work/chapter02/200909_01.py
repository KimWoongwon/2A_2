s = " "
print("문장을 입력해주세요. 'q' 입력시 종료됩니다.")
while s != 'q':
    s = input(" ")
    if s[-1] == '.':
        print("온점을 잘 입력했습니다.")
    else:
        print("온점을 입력하세요.")
