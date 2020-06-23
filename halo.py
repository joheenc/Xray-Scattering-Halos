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
houghlib = CDLL(f'{filepath}{filename}.so')

c_int_p = POINTER(c_int32)

xs = xs.astype(np.int32)
ys = ys.astype(np.int32)

cxs = xs.ctypes.data_as(c_int_p)
cys = ys.ctypes.data_as(c_int_p)

houghlib.hough.argtypes = [c_int_p, c_int_p, c_int32]
houghlib.hough.restype = c_int_p
halo = houghlib.hough(cxs, cys, photonCount)
center = (halo[0], halo[1])
r = halo[2]
houghlib.free(halo)
print('\nHighest accumulator halo: center = ' + str(center) + ', r = ' + str(r))

#draw the image with the most likely halo superimposed
import matplotlib.pyplot as plt
fig, ax = plt.subplots()
ax.set_facecolor('black')
plt.scatter(xs, ys, s=0.1, color='white')
ax.add_artist(plt.Circle(center, r, color='r', fill=False))
plt.show()

