import numpy
import numpy as np

a = np.array([23,45,67,7,2,30,34,82])
print('a = {}'.format(a))
print('Max = {}'.format(a.max()))
print('Min = {}'.format(a.min()))
print('Avg = {}\n'.format(a.mean()))

b = np.random.randint(0,100,size=10)
print('b = {}'.format(b))
print('Max = {}'.format(b.max()))
print('Min = {}'.format(b.min()))
print('Avg = {}\n'.format(b.mean()))

c = np.append(a,b)
print('c = {}\n'.format(c))