import os
from halo import halo
datadir = '../data/obs/'
for fname in os.listdir(datadir):
    halo(datadir+fname, saveimg=True, outfile='outfiles/'+fname.replace('.evt.gz', '.png'))
