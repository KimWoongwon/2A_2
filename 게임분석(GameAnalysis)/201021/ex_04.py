## 클래스운용법
class block_factory:
    def __init__(self, company, color, shape):
        self.company = company
        self.color = color
        self.shape = shape
    def make_pink(self):
        self.color = "PINK"
    def printinfo(self):
        print("제조회사 : {0:10} 블록컬러 : {1:10}\t 블록모양 : {2:10}"
              .format(self.company, self.color, self.shape))


block = block_factory('Gole', 'blue', 'long')
block.printinfo()

newblock2 = block_factory("Smith", "Black", "Short")
newblock2.printinfo()

newblock2.make_pink()
newblock2.printinfo()

