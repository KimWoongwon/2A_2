import random
number = ['A', 2,3,4,5,6,7,8,9,10,'J','Q','K']
figure = ['♠', '♥', '♣', '◆']

deck = []
for i in range(len(figure)):
    for j in range(len(number)):
        deck.append(str(number[j]) + figure[i])

while True:
    flag = input("엔터키를 누르면 카드가 나옵니다. 'q'를 누르면 종료합니다.")
    if flag == 'q':
        print('프로그램을 종료합니다.')
        break
    nowcard = deck[random.randrange(0,len(deck))]
    print('이번에 나온 카드는',nowcard,'입니다.\t 남은 카드수 : ', len(deck))
    deck.remove(nowcard)
    if len(deck) <= 0:
        print('남은 카드가 없습니다.')
        break




