import numpy as np

a = np.array([1,3,4])
print(a)
print()

print(np.insert(a, 1, 2))
print()

a = np.array([[1, 1], [2, 2], [3, 3]])
print(a)
print()

print(np.insert(a, 1, 4, axis=0))
print()

print(np.insert(a, 1, 4, axis=1))
print()