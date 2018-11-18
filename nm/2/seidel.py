import numpy as np
import copy

x_correct = np.array([0.4, -0.0946, -0.54]).reshape(1, 3)

N = 3
A = np.array([[3.6, 1.8, -4.7], [2.7, -3.6, 1.9], [1.5, 4.5, 3.3]])
b = np.array([3.8, 0.4, -1.6])
eps = 0.0001

print(f"A\n{A}")
print(f"b\n{b}")

ATA = np.dot(np.transpose(A), A)
ATb = np.dot(np.transpose(A), b)

left = np.zeros((3, 3), dtype=np.float)
left[np.arange(N), np.arange(N)] = 1
values = ATA[np.arange(N), np.arange(N)].reshape(N, 1)

b_right = ATb.reshape((N, 1)) / values

x = np.zeros((N, 1))

right = (ATA / values)
right[np.arange(N), np.arange(N)] = 0

while True:
    old_x = copy.deepcopy(x)

    x[0][0] = b_right[0][0] - np.dot(right[0], x)
    x[1][0] = b_right[1][0] - np.dot(right[1], x)
    x[2][0] = b_right[2][0] - np.dot(right[2], x)

    if np.linalg.norm(x-old_x) < eps:
        break

print(f"x\n{x}\n")
