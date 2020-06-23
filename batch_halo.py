from halo import halo
import glob
import os

data_dir = "/Users/hunterholland/Documents/Research/Laidlaw/Data/Modified"

evt2 = glob.glob(f"{data_dir}/**/Chandra/**/*evt2.fits*", recursive=True)
cleaned_event = glob.glob(
    f"{data_dir}/**/Swift/xrt/event/sw*po_cl.evt.gz", recursive=True)

# for file in cleaned_event:
#     halo(cleaned_event[0], saveimg=True)

halo(cleaned_event[1])
# halo("/Users/hunterholland/Documents/Research/Laidlaw/Data/Modified/L1517/Chandra/primary/evt2_f1.fits")
