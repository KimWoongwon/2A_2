import numpy
import numpy as np

a = np.arange(1, 11)
print('a = {}'.format(a))

b = a
b[::-1].sort()
print('b = {}'.format(b))
print()

c = b.reshape(2,5)
print('c = \n{}'.format(c))
print()

d = c.reshape(5,2)
print('d = \n{}'.format(d))
print()