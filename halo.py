import os
from ctypes import CDLL

so_filename = "fuzzyhough_nottxt.so"
os.system(
    f"cd Pipelines/Xray-Scattering-Halos; gcc -shared -o {so_filename} -fPIC fuzzyhough_nottxt.c")

so_filepath = f"/Users/hunterholland/Documents/Research/Laidlaw/Pipelines/Xray-Scattering-Halos/{so_filename}"
halo_object = CDLL(so_filepath)

# halo_object.hough(2darray)
