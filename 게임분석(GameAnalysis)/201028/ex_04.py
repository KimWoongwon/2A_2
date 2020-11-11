## 비공개 클래스 속성 사용하기
class Knight:
    __item_limit = 10

    def Print_item(self):
        print(Knight.__item_limit)

x = Knight()
x.Print_item()