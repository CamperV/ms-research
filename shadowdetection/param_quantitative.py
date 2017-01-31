# Not to be used outside of ./batch_quantitative.sh
# a python script for running and collecting the outputs of C++ program 'quantitative.'

import sys
import subprocess
import matplotlib
import os
try:
    from configparser import ConfigParser
except ImportError:
    from ConfigParser import ConfigParser  # ver. < 3.0

def frange(x, y, jump=1.0):
  '''Range for floats.'''
  i = 0.0
  x = float(x)  # Prevent yielding integers.
  x0 = x
  epsilon = jump / 2.0
  yield x  # yield always first value

  while x + epsilon < y:
    i += 1.0
    x = x0 + i * jump
    yield x

try:
  indir = sys.argv[1]

  paramline = sys.argv[2].split(",")
  section = paramline[0]
  param = paramline[1]

  rangeline = sys.argv[3].split(",")
  upperrange = rangeline[0]
  step = rangeline[1]
except:
  print "usage: python param_quantitative.py [indir] [method,.ini param] [range ceil,step]"
  print "ex: python param_quantitative.py ./samples/PETs1 P,coneAngle 3.5,0.1"
  sys.exit(0)

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

# WITHOUT ConfigParser
for line in open('default.ini'):
  if param in line:
    splitline = line.strip("\n").split(" ")
    param_val = float(splitline[-1])
    print "Init val of " + param + ": " + str(param_val)

# ConfigParser impl
# ---
config = ConfigParser()
config.optionxform = str  # make sure to not strip capitals
config.read('default.ini')

# read values from a section
#param_val = config.getfloat(psection, param)

listing = os.listdir(frames_path)
for infile in sorted(listing):
  fr = frames_path+infile
  bg = bgs_path+infile
  sh = shadows_path+infile

  # for each param_val in range 0-upperrange
  for val in frange(0, float(upperrange), float(step)):
    print val
    # update existing value
    config.set(psection, param, val)
    
    # save to a file
    with open('params.ini', 'w') as configfile:
        config.write(configfile)
  
    # now that params are set, run SD
    process = subprocess.Popen(["./bin/quantitative", fr, bg, sh], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  
    for line in process.stderr:
      splitline = line.strip("\n").split(",")
  
      print splitline[0] + " " + splitline[1] + " " + splitline[2]
        
      # store filename (x-axis)
      # store detection rate (y-axis)
      # store discrimination rate (y-axis2)
      # store changing param (z-axis)
      # plot correlated variable (y-axis3)
  
      #if(splitline[0] == "C"):
      #  cDetect += float(splitline[1])
      #  cDiscrim += float(splitline[2])
      #elif(splitline[0] == "P"):
      #  pDetect += float(splitline[1])
      #  pDiscrim += float(splitline[2])
      #elif(splitline[0] == "G"):
      #  gDetect += float(splitline[1])
      #  gDiscrim += float(splitline[2])
      #elif(splitline[0] == "SR"):
      #  srDetect += float(splitline[1])
      #  srDiscrim += float(splitline[2])
      #elif(splitline[0] == "LR"):
      #  lrDetect += float(splitline[1])
      #  lrDiscrim += float(splitline[2])
  
