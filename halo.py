import os
from ctypes import CDLL, POINTER, c_int32
import matplotlib.pyplot as plt
from astropy.io import fits
import numpy as np
import faulthandler

def halo(filepath, saveimg=False, outfile='out.png'):
    """This method iterates through the input event file and uses an implementation of the Fuzzy Hough Transform
    to determine a series of the most optimal halos of incident photons. It then superimposes the most optimal
    halo onto the image and returns a PNG file which gets saved to the event file's directory.
    """
    
    faulthandler.enable()

    # get event data from FITS file
    hdul = fits.open(filepath)
    xs = np.array(hdul[1].data['X'])
    xs -= np.min(xs)
    ys = np.array(hdul[1].data['Y'])
    ys -= np.min(ys)
    photonCount = len(xs)

    # cpath should be the absolute path to the C file which does the Fuzzy Hough calculation (halo_calculation.c)
    cpath = "halo_calculation"
    cfilename = os.path.basename(cpath)
    # create C library files in the file's directory
    os.system(
        f'gcc -c -fPIC -lm {cpath}.c -o {cpath}.o; gcc {cpath}.o -shared -o {cpath}.so')
    houghlib = CDLL(f'{cpath}.so')

    c_int_p = POINTER(c_int32)

    # convert numpy arrays from int16 to a C-compatable int32
    xs = xs.astype(np.int32)
    ys = ys.astype(np.int32)

    # cast arrays as ctypes
    cxs = xs.ctypes.data_as(c_int_p)
    cys = ys.ctypes.data_as(c_int_p)

    # set the types for the inputs and outputs to the C hough function
    houghlib.hough.argtypes = [c_int_p, c_int_p, c_int32]
    houghlib.hough.restype = c_int_p
    # call the hough function and store the returned, "most likely" parameters in halo
    haloinfo = houghlib.hough(cxs, cys, photonCount)
    # stores the (x, y) coordinates  of the center
    center = (haloinfo[0], haloinfo[1])
    r = haloinfo[2]  # stores the radius
    sigma = haloinfo[3]
    print('\nHighest accumulator halo: center = ' +  # note that this only outputs the single highest accumulator-value halo
          str(center) + ', r = ' + str(r) + ', sigma = ' + str(sigma))

    # create image with the most likely halo superimposed
    _fig, ax = plt.subplots()
    ax.set_facecolor('black')
    plt.xlim(0, np.max(xs))
    plt.ylim(0, np.max(ys))
    plt.scatter(xs, ys, s=0.1, color='white')
    ax.add_artist(plt.Circle(center, r, color='r', fill=False))

    houghlib.free(haloinfo)
    # save image
    if saveimg == True:  # save PNG
        plt.savefig(outfile, dpi=250, format="png")
    # show image
    else:
        plt.show()
