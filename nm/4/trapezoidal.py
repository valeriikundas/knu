def f(x):
    return x*x+1

a=-1
b=2
h=0.1

sum=0

i=a
while i<b:
    f_i=f(i)
    f_ih=f(i+h)
    
    sum += (f_i+f_ih)/2 * h
    i+=h

print(sum)
    

# pohibka E(f)=(b-a)^3/(12*n*n) * M, M=max(f''(x), x in [a,b])