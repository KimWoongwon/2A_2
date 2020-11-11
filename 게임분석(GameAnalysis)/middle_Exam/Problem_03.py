code = ""
total = 0

while True:

    code = input('주민등록번호(13자리) : ')

    if len(code) != 13:
        print('잘못된 주민등록번호 입니다.')
        break

    for i in range(len(code) - 1):
        total += int(code[i]) * (i % 8 + 2)

    total %= 11
    total = 11 - total

    if total == int(code[12]):
        print('정상적인 주민등록번호 입니다.')
    else:
        print('정상적이지 않은 주민등록번호 입니다.')