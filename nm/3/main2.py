# System of non linear equations
from math import log, fabs
from copy import deepcopy
import numpy as np


def System(N, X):
    if N == 1:
        return -0.1 * X[1]**2 - 0.2 * X[2]**2 + 0.3
    elif N == 2:
        return -0.1 * X[1]**2 + 0.1 * X[1] * X[2] + 0.7


def MPI(n, m, X, eps=1e-3):
    k = 0
    while True:
        d = 0
        b = deepcopy(X)
        A = deepcopy(b)
        A[1] = System(1, X)
        X[1] = A[1]
        A[2] = System(2, X)
        X[2] = A[2]
        A = deepcopy(b)
        for i in range(1, n + 1):
            d1 = fabs(X[i] - A[i])
            if d < d1:
                d = d1
        k += 1
        if (d <= eps):
            print("Solution is ", X, "\nnumber of iteration=")
        break
        A = deepcopy(X)
        if k > m:
            print("Процес розбігається!")


X = np.array([0., 0.25, 0.75])
n = 2
m = 10
MPI(n, m, X)
