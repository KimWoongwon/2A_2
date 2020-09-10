s = " "
idx = 0
print("문서를 작성하세요.")
s = input(" ")
## finished files are the result of years of scientifics study combined with the experience of year ##
print("찾을 문자를 입력하세요. : ") ## f
c = input('')

for i in range(s.count(c)):
    if s.count(c) > 0:
        idx = s.find(c,idx)
        print(s[idx], idx + 1 , ' / ', s.count(c))
        idx += 1