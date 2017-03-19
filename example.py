import sys
import numpy
from PIL import Image

img = Image.open(sys.argv[1]).convert('L')

im = numpy.array(img)
fft_mag = numpy.abs(numpy.fft.fftshift(numpy.fft.fft2(im)))

visual = numpy.log(fft_mag)
visual = (visual - visual.min()) / (visual.max() - visual.min())

result = Image.fromarray((visual * 255).astype(numpy.uint8))
result.save('out.bmp')

