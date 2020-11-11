#커피 주문 시스템

def Order(kind, name = ""):
    if name == "":
        name = Order.OrderCount + 1

    print("{0} 고객님, 주문하신 {1} 나왔습니다.\n".format(name,kind))
    Order.OrderCount += 1

Order.OrderCount = 0

while True:
    coffee = input("커피 종류를 입력하세요 : ")
    if coffee == "마감":
        break

    name = input("고객명을 입력하세요 : ")

    Order(coffee,name)
