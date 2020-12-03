import numpy as np

a = np.arange(0, 6).reshape(3,2)
print(a)

print(a.sum())
print()

print(a.sum(axis=0))
print()

print(a.sum(axis=1))
print()

print(a.min(axis=0))
print()

print(a.min(axis=1))
print()

print(a.max(axis=0))
print()

print(a.max(axis=1))
print()