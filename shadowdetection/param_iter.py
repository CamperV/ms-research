# Not to be used outside of ./batch_quantitative.sh
# a python script for running and collecting the outputs of C++ program 'quantitative.'

import sys
import subprocess
import os
try:
    from configparser import ConfigParser
except ImportError:
    from ConfigParser import ConfigParser  # ver. < 3.0
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import csv

# range for floats
def frange(x, y, jump=1.0):
  i = 0.0
  x = float(x)  # Prevent yielding integers.
  x0 = x
  epsilon = jump / 2.0
  yield x  # yield always first value
  while x + epsilon < y:
    i += 1.0
    x = x0 + i * jump
    yield x

# take in args
try:
  indir = sys.argv[1]

  paramline = sys.argv[2].split(",")
  section = paramline[0]
  param = paramline[1]

  rangeline = sys.argv[3].split(",")
  upperrange = rangeline[0]
  step = rangeline[1]
except:
  print "usage: python param_quantitative.py [indir] [method,.ini param] [range ceil,step] [mode (graph/optimal)]"
  print "ex: python param_quantitative.py ./samples/PETs1 P,coneAngle 3.5,0.1, graph"
  sys.exit(0)

# build paths
frames_path = indir + "/frames/"
bgs_path = indir + "/bgs/"
shadows_path = indir + "/shadows/"

# check section/value
if(section == "C"):
  psection = "ChromacityShadRemParams"
elif(section == "P"):
  psection = "PhysicalShadRemParams"
elif(section == "G"):
  psection = "GeometryShadRemParams"
elif(section == "SR"):
  psection = "SrTextureShadRemParams"
elif(section == "LR"):
  psection = "LrTextureShadRemParams"
else:
  print "Please enter valid sectionname."
  exit(0)

# check if param is present/what is default
# WITHOUT ConfigParser
for line in open('default.ini'):
  if param in line:
    splitline = line.strip("\n").split(" ")
    param_val = float(splitline[-1])
    print "Init val of " + param + ": " + str(param_val)

print "\n"
print indir + "," + str(paramline) + "," + str(rangeline)

# ConfigParser impl
config = ConfigParser()
config.optionxform = str  # make sure to not strip capitals
config.read('default.ini')

# init listdir
listing = os.listdir(frames_path)

print "Processing..."
# processing
for infile in sorted(listing):
  fr = frames_path+infile
  bg = bgs_path+infile
  sh = shadows_path+infile

  # for each param_val in range 0-upperrange
  for val in frange(0, float(upperrange), float(step)):

    # update existing value
    config.set(psection, param, val)
  
    # save to a file
    with open('params.ini', 'w') as configfile:
        config.write(configfile)
  
    # now that params are set, run SD
    process = subprocess.Popen(["./bin/quantitative", fr, bg, sh], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  
    csvname = indir.strip("/")+"_"+str(infile)[:-4]+".csv"
    for line in process.stderr:
      splitline = line.strip("\n").split(",")
      if(splitline[0] == section):

        # val,detection,discrimination
        with open(csvname, 'a') as csvfile:
          paramwriter = csv.writer(csvfile, delimiter=',')
          paramwriter.writerow([str(val), splitline[1], splitline[2]])
