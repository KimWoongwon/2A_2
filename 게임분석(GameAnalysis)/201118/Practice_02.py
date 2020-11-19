import numpy
import numpy as np

a1 = np.arange(1, 13).reshape(2,6)
print('a1 = \n{}'.format(a1))
print()

a2 = np.arange(1, 31).reshape(3,10)
print('a2 = \n{}'.format(a2))
print()

a3 = a2.reshape(6,5)
print('a3 = \n{}'.format(a3))
print()

a4 = a3.transpose()
print('a4 = \n{}'.format(a4))
print()
