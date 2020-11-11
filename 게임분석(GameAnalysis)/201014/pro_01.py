# 원점과 임의의 점의 거리구하기

def Distance(x = 0,y = 0):
    if x == 0 and y == 0:
        x = int(input('x좌표를 입력하세요 : '))
        y = int(input('y좌표를 입력하세요 : '))

    distance = (x**2 + y**2) ** 0.5
    print(distance)
    return distance

Distance(3,4)

Distance()
