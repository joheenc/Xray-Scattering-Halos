from PIL import Image, ImageDraw

x = 579
y = 361
r = 225

sample_image_filepath = 'sample.png'

image = Image.open(sample_image_filepath)
draw = ImageDraw.Draw(image)
draw.arc((x-r, y-r, x+r, y+r), 0, 360)
image.save("output.png")
