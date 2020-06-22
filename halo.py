from ctypes import *
from astropy.io import fits
import numpy as np
import os

hdul = fits.open('event.evt.gz')
xs = hdul[1].data['X']
ys = hdul[1].data['Y']
photonCount = len(xs)

filename = "fuzzyhough_notxt"
filepath = "./"

os.system(f'gcc -c -fPIC -lm {filename}.c -o {filename}.o')
os.system(f'gcc {filename}.o -shared -o {filename}.so')
halo_object = CDLL(f'{filepath}{filename}.so')

print(xs[0])
c_int_p = POINTER(c_int)
halo_object.hough.argtypes = [c_int_p, c_int_p, c_int]
halo_object.hough(xs.ctypes.data_as(c_int_p), ys.ctypes.data_as(c_int_p), photonCount)
