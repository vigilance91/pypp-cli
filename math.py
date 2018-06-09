##@package 
#@author Tyler R. Drury
#@brief example script for executing c math functions

#polar/cartesian coordinate conversion
from cmath import phase
from cmath import polar
from cmath import rect
#power and log functions
from cmath import exp
from cmath import log
from cmath import log10
from cmath import sqrt
#trig
from cmath import sin
from cmath import cos
from cmath import tan
#
from cmath import asin
from cmath import acos
from cmath import atan
#hyperbolic
from cmath import sinh
from cmath import cosh
from cmath import tanh
#
from cmath import asinh
from cmath import acosh
from cmath import atanh
#constants
from cmath import pi as PI
from cmath import e as E
#
from cmath import isnan
from cmath import isinf
#
#import dateTime, ctime
#
#TODO: define vector library
#TODO: define matrix library
#TODO: define quaternion library
#
twoPI = 2 * PI

INC = twoPI / 8

#assert(twoPI % INC == 0)

angles = []
i = 0 * PI

while i <= twoPI:
    angles.append(i)
    i += INC

def call(cb):
    for a in angles:
        res = cb(a)
        aStr = str(a)

        if isinf(res):
            print('angle: ' + aStr + ', value: Infinity')
            return
        elif isnan(res):
            print('angle: ' + aStr + ', value: NaN')
            return

        print('angle: ' + aStr + ', value: ' + str(res))
#
#print('starting execution')    #TODO: timestamp
#
print('sin')
call(sin)
print('cos')
call(cos)
print('tan')
call(tan)
#
print('asin')
call(asin)
print('acos')
call(acos)
print('atan')
call(atan)
#
print('sinh')
call(sinh)
print('cosh')
call(cosh)
print('tanh')
call(tanh)
#
print('asinh')
call(asinh)
print('acosh')
call(acosh)
print('atanh')
call(atanh)
#
##print('ending execution')    #TODO: timestamp
#