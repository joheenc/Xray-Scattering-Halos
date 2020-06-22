from ctypes import CDLL, POINTER, c_int32
# import ctypes
from astropy.io import fits
import numpy as np
import os

hdul = fits.open('event.evt.gz')
xs = np.array(hdul[1].data['X'])
ys = np.array(hdul[1].data['Y'])
photonCount = len(xs)

filename = "fuzzyhough_notxt"
filepath = "./"

os.system(f'gcc -c -fPIC -lm {filename}.c -o {filename}.o')
os.system(f'gcc {filename}.o -shared -o {filename}.so')
halo_object = CDLL(f'{filepath}{filename}.so')

print(xs[0])
c_int_p = POINTER(c_int32)

xs = xs.astype(np.int32)
ys = ys.astype(np.int32)

cxs = xs.ctypes.data_as(c_int_p)
cys = ys.ctypes.data_as(c_int_p)

halo_object.hough.argtypes = [c_int_p, c_int_p, c_int32]
halo_object.hough(cxs, cys, photonCount)
