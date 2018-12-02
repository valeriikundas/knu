import time


def f(x, y):
    return y*y/(x+1)


h = 0.1
xi = 1
xhi = 1.5
yi = 1


i = 0
while True:
    i += 1
    print(f"i={i}, xi={round(xi,4)}, yi={round(yi,4)}, f={round(f(xi, yi),4)}, h*f={round(h*f(xi, yi),4)}")
    xi = xi+h
    yi = yi + h * f(xi, yi)
    if xi > xhi:
        break
