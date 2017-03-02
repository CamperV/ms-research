import sys
import numpy as np
from matplotlib import pyplot as plt
import cv2

# take in args
try:
    infile = sys.argv[1]
except:
    print "usage: python dominance.py [in]"
    sys.exit(0)

img = cv2.imread(str(infile))

chans = cv2.split(img)
colors = ("b", "g", "r")
plt.figure()
plt.title("'Flattened' Color Histogram")
plt.xlabel("Bins")
plt.ylabel("# of Pixels")
features = []

# loop over the image channels
for (chan, color) in zip(chans, colors):
    hist = cv2.calcHist([chan], [0], None, [256], [0, 256])
    features.extend(hist)

    # plot the histogram
    plt.plot(hist, color = color)
    plt.xlim([0, 256])

plt.show()
print "flattened feature vector size: %d" % (np.array(features).flatten().shape)
