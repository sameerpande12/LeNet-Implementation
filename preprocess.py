import cv2
import numpy
import sys

filename = sys.argv[1]

img = cv2.imread(filename,0)

if img.shape != [28,28]:
    img2 = cv2.resize(img,(28,28))
    img = img2.reshape(28,28,-1);
else:
    img = img.reshape(28,28,-1);
#revert the image,and normalize it to 0-1 range
img = 1.0 - img/255.0

filename = sys.argv[2]
numpy.savetxt(filename,img[:,:,0],fmt='%.18e',delimiter=' ',newline='\n',header='',footer='',comments='#',encoding=None)
