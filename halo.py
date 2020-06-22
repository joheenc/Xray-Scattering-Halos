import os
from ctypes import CDLL
from astropy.io import fits

hdul = fits.open('event.evt.gz')
xs = hdul[1].data['X']
ys = hdul[1].data['Y']
photonCount = len(xs)

filename = "fuzzyhough_notxt"
os.system(f"gcc -shared -o {filename}.so -fPIC {filename}.c")

so_filepath = f"/Users/hunterholland/Documents/Research/Laidlaw/Pipelines/Xray-Scattering-Halos/{filename}.so"
halo_object = CDLL(so_filepath)

halo_object.hough(xs, yx, photonCount)
