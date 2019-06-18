import numpy as np
import matplotlib.pyplot as plt
from skimage import data, color
from skimage.util import img_as_ubyte
from skimage.io import imread
from PIL import Image

img = imread('sample_images/sample1.png')
print(img.shape)
imggray = color.rgb2gray(img)
imgubyte = img_as_ubyte(imggray)
print(imgubyte.shape)

# np.savetxt("img.txt", img)
# np.savetxt("ubyte.txt", imgubyte)

plt.imsave('image_outputs/recreated.png', imgubyte, cmap='Greys')

normimg = imgubyte
for x in range(len(normimg)):
    for y in range(len(normimg[1])):
        if normimg[x][y] != 0:
            normimg[x][y] = 1

# np.savetxt("normimg.txt", normimg, fmt='%1.0f')
plt.imsave('image_outputs/normrecreated.png', normimg, cmap='Greys')
