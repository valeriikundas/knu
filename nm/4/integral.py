def f(x):
    return x*x+1


a = -1
b = 2
h = 0.1

sum = 0

i = a
while i < b:
    f_i = f(i)
    f_ih = f(i+h)
    f_avg = (f_i+f_ih)/2
    sum += h*f_avg
    i += h

print(sum)


# pohibka E(f)=(b-a)^3/24 * f''(n), n in [a,b]
