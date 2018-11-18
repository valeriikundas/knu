def F(x, coefs):
    return sum([val*(x**i) for i, val in enumerate(coefs)])


def f(x):
    return F(x, [-29, 44, -10, 1])


def g(x):
    return F(x, [-29, 45, -10, 1])


def df(x):
    return F(x, [44, -20, 3])


def ddf(x):
    return F(x, [-20, 6])


a = 0.5
b = 1
print(f"a = {a}, b = {b}")
eps = 0.0001

fa = df(0.5)
fb = df(1)
x0 = 10/3  # f''(x0) == 0
fx0 = df(x0)

fmin = min(fa, fb, fx0)

fmax = fa
xmax = a
if fb > fmax:
    xmax = b
    fmax = fb
if fx0 > fmax:
    xmax = x0
    fmax = fx0

print(f"fmin = {fmin}, fmax = {fmax}")

lambda_ = 1/fmax
print(f"lambda = {lambda_}")

print(f"f(a) * f(b) = {f(a)*f(b)} < 0  => the solution is in [{a},{b}]")

xi = 1000
xj = xmax

while abs(xi-xj) > eps:
    xi = xj
    xj = F(xi, list(map(lambda x: x*(-lambda_), [-29, 44, -10, 1]))) + xi
    print(f"xi={xi}, xj={xj}")
    print(f"f(Xn)={f(xj)}")


print(f"solution x={xj}")
