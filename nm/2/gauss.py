import numpy as np
import copy

x_correct = np.array([-4, -39, 44]).reshape(1, 3)

N = 3
A = np.array([[1, 2, 3], [3, 5, 7], [1, 3, 4]])
b = np.array([3, 0, 1]).reshape(N, 1)

C = np.hstack((A, b))
C = np.array(C, dtype=np.float)

print(f"starting matrix\n{C}\n")

triangle_matrix = np.zeros_like(C, dtype=np.float)
undeleted_columns = list(range(N))
n = N


for it in range(N):
    a = C[:n, :n]
    imax = np.argmax(np.abs(a))
    p, q = np.unravel_index(imax, a.shape)

    m = -a[:, q] / a[p, q]

    extra = np.dot(m.reshape(n, 1), C[p, :].reshape(1, n+1))
    extra[p, :] = 0
    C = C + extra

    triangle_matrix[it, -1] = C[p, -1]
    triangle_matrix[it, np.array(undeleted_columns)] = C[p, :-1]
    undeleted_columns.remove(undeleted_columns[q])

    C = np.delete(C, p, 0)
    C = np.delete(C, q, 1)

    n -= 1


print(f"triangle matrix \n{triangle_matrix}\n")
tm = copy.copy(triangle_matrix)

x_solution = np.zeros_like(b, dtype=np.float)

it = N - 1
while it >= 0:
    xId = np.nonzero(triangle_matrix[it, :-1])[0][0]
    left = triangle_matrix[it, xId]
    right = triangle_matrix[it, -1]
    x_solution[xId] = right/left

    for i in range(N):
        triangle_matrix[i, -1] += -triangle_matrix[i, xId]*x_solution[xId][0]
        triangle_matrix[i, xId] = 0

    it -= 1


print(f"x = {x_solution.reshape(-1)}")
