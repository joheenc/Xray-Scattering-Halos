import os
from halo import halo
datadir = '<your data directory goes here>'
for fname in os.listdir(datadir):
    halo(datadir+fname, saveimg=True, outfile='outfiles/'+fname.replace('.evt.gz', '.png'))
