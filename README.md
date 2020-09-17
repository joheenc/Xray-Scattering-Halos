halo_calculation.c contains code for the FHT (fuzzy hough transform, https://www.sciencedirect.com/science/article/pii/016786559490068X)
halo.py is the Python interface to the C code (useful because Python is useful for working with FITS files)
batchrun.py is an example of how you might run halo.py on several FITS files located in another directory
outfiles/ contains the results of a sample run of the algorithm on 68 Swift observations
