from skimage import data, color
from skimage.util import img_as_ubyte
from skimage.io import imread

sample_image_filepath = 'sample.png'

img = imread(sample_image_filepath)
normimg = img_as_ubyte(color.rgb2gray(img))

# plt.imsave('image_outputs/recreated.png', normimg, cmap='Greys') #if you want to check that the image was read correctly

for x in range(len(normimg)):       #'normalize' the image--i.e. set the non-photon pixels to 0 and photons to 1
    for y in range(len(normimg[1])):
        if normimg[x][y] != 0:
            normimg[x][y] = 1

for x in range(len(normimg)):       #output the normalized image
    for y in range(len(normimg[1])):
        print(normimg[x][y], end="")
    print()


# plt.imsave('image_outputs/normrecreated.png', normimg, cmap='Greys') #if you want to see the results of normalizing the image
