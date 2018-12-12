from numpy.polynomial import hermite
import numpy

x = numpy.linspace(-10, 10)
err = numpy.random.randn(len(x))/10
y = hermite.hermval(x, [1, 2, 3]) + err

print(x)
print(y)
c = hermite.hermfit(x, y, 2)
print(c)

predicted = hermite.hermval(x, c)

print(numpy.linalg.norm(predicted-y))
