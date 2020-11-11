
CarList = ['Gene', 'Lex', 'Infini', 'Lambor', 'Linc']
print(CarList, '\n')

CarList.append('Merce')
print('append Merce')
print(CarList, '\n')

CarList.remove('Lex')
print('remove Lex')
print(CarList, '\n')

CarList.append('Gene2020')
print('append Gene2020')
print(CarList, '\n')

print('두번째부터 네번째 개발한 모델 출력')
print(CarList[1:4])