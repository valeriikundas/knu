import numpy as np

a = np.random.rand(3, 3)

print(f"det = {np.linalg.det(a)}")


inv = np.linalg.inv(a)

cond = np.linalg.norm(a)+np.linalg.norm(inv)

print(f"cond = {cond}")
