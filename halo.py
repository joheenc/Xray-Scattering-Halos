import os
from ctypes import CDLL

filename = "fuzzyhough_notxt"
os.system(f"gcc -shared -o {filename}.so -fPIC {filename}.c")

so_filepath = f"/Users/hunterholland/Documents/Research/Laidlaw/Pipelines/Xray-Scattering-Halos/{filename}.so"
halo_object = CDLL(so_filepath)

# halo_object.hough(2darray)
