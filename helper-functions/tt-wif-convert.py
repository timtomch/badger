# This script takes a PNG image as input and saves it in the WyoLum Image Format (WIF)
# for display on the badgeR e-ink board, see http://wyolum.com/projects/badger/
# By Thomas Guignard tom@timtom.ca - February 2015
# Based on greyscale.py from the WyoLum EPD library https://github.com/wyolum/EPD
#
# Usage: tt-wif-convert.py -i <inputfile> -o <outputfile> -c <contrast> -b <brightness>
#
# TODO - Handle different sizes. For the time being, all images will be resized to 264x176 pixels

import PIL.Image as Image
import struct
import sys
import getopt
import ImageEnhance

default_size=tuple((264,176))

def myEqualize(im, contrast=1.0, brightness=1.0):
    if im is not None:
        im = im.convert('L')
        contr = ImageEnhance.Contrast(im)
        im = contr.enhance(contrast)
        bright = ImageEnhance.Brightness(im)
        im = bright.enhance(brightness)
        #im.show()
    return im

def towif(im, outfn, width, height):
    ''' 
    image should be sized already (2.7" display=264x276 pixels) in "1" format
    '''
    f = open(outfn, 'w')
    f.write(struct.pack('HH', height, width))
    for j in range(height):
        for i in range(0, width, 8):
            byte = 0
            for bit_i in range(8):
                try:
                    bit = im.getpixel((i + bit_i, j)) < 255
                except:
                    bit = False
                #sys.stdout.write(' X'[bit])
                byte |= bit << bit_i
            f.write(struct.pack('B', byte))

# Parse input and call the conversion functions
def main(argv):
	# Default values
	contrast = 1
	brightness = 1
	inputfile = ''
	outputfile = 'OUTPUT.WIF'
	try:
		opts, args = getopt.getopt(argv,"hi:o:c:b:",["infile=","outfile=","contrast=","brightness="])
	except getopt.GetoptError:
		print 'Usage: tt-wif-convert.py -i <inputfile> -o <outputfile> -c <contrast> -b <brightness>'
		sys.exit(2)
	for opt, arg in opts:
		if opt == '-h':
			print 'Usage: tt-wif-convert.py -i <inputfile> -o <outputfile> -c <contrast> -b <brightness>'
			sys.exit()
		elif opt in ("-i", "--infile"):
			inputfile = arg
		elif opt in ("-o", "--outfile"):
			outputfile = arg
		elif opt in ("-c", "--contrast"):
			contrast = arg
		elif opt in ("-b", "--brightness"):
			brightness = arg	
	print 'Input file is ', inputfile
	print 'Output file is ', outputfile
	print 'Contrast is ', contrast
	print 'Brightness is ', brightness
	
	# Open original image
	image_file = Image.open(inputfile)
	org_size = image_file.size
	
	# Check if image needs to be resized
	if org_size != default_size:
		image_file = image_file.resize(default_size) 
	
	# Convert to monochrome (greyscale)
	image_file = image_file.convert('L')
	
	# Correct brightness/contrast
	# This is not working as it should...
	#image_file = myEqualize(image_file, contrast, brightness)
	
	# Convert to black and white
	image_file = image_file.convert('1')
	# Preview black and white image
	image_file.show()
	
	# Convert to WIF
	towif(image_file, outputfile,default_size[0],default_size[1])

# Call the main function on load
if __name__ == "__main__":
   main(sys.argv[1:])