# Character generator Python script
# Finn Beckitt-Marshall, 201091782
# March 2023

# Imports
from PIL import Image, ImageFont, ImageDraw
import numpy as np

# Constants
# CHARSIZE: Character size in pixels (tuple)
CHARSIZE = (8, 8)
# WHITE:    White colour tuple (red, green and blue all 255)
WHITE = (255, 255, 255)
# BLACK:    Black colour tuple (red, green and blue all 0)
BLACK = (0, 0, 0)
# OFFSET:   Tuple containing the character offset (i.e. how much to offset
#           the character position by)
OFFSET = (-1, -4)

# Create an empty array of pixels
pixel_data = []

# Create an image of size 8x8 with a white background.
img = Image.new('RGB', CHARSIZE, WHITE)

# Add drawing functionality
draw = ImageDraw.Draw(img)
font = ImageFont.truetype('pixeldroidMenuRegular.ttf', 16)
# From the following StackOverflow post, setting the fontmode to '1' removes
# antialiasing - we don't want it, as we want sharp pixel values:
# https://stackoverflow.com/questions/5747739/how-to-render-non-anti-aliased-font-to-internal-image
draw.fontmode = '1'

# ASCII codes 32 to 126 (inclusive) cover the standard letters.
for character in range(32, 127):
    print('Creating pixel representation of ASCII character {} - {}'.format(character, chr(character)))
    draw.rectangle([0, 0, 8, 8], WHITE)
    draw.text(OFFSET, chr(character), BLACK, font=font)

    pixel_val = 0
    for y in range(0, CHARSIZE[0]):
        for x in range(0, CHARSIZE[1]):
            print('\tGetting pixel value at ({}, {})'.format(x, y))
            if img.getpixel((x, y)) == WHITE:
                print('\t\tWhite pixel')
            else:
                print('\t\tBlack pixel')
                # Set the pixel at the correct position to 1 to generate a
                # black pixel
                pixel_val |= 1 << (CHARSIZE[0] * x + y)

    print('Pixel value = {:x}'.format(pixel_val))
    pixel_data.append(pixel_val)

# Write the pixel data into a MIF file.
# MIF format used - https://media.oregonstate.edu/media/t/1_u1q55wz3/183308761
out = open('chardata.mif', 'w')
# Add the depth (number of ASCII characters, equal to the size of the
# pixel_data array)
out.write('DEPTH = {};\n'.format(len(pixel_data)))
# Add the width (CHARSIZE[0] * CHARSIZE[1] bytes)
out.write('WIDTH = {};\n'.format(CHARSIZE[0] * CHARSIZE[1]))
# Add the address and data radixes
out.write('ADDRESS_RADIX = HEX;\n')
out.write('DATA_RADIX = HEX;\n\n')
# Add the content begin lines
out.write('CONTENT\n')
out.write('BEGIN\n')

# For loop to write the data into the MIF file
for character in range(0, len(pixel_data)):
    out.write('{:x} : {:x};\n'.format(character, pixel_data[character]))

out.write('END')

out.close()
