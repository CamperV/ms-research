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

# take in args
try:
  indir = sys.argv[1]

  paramline = sys.argv[2].split(",")
  section = paramline[0]
  param = paramline[1]
except:
  print "usage: python param_model.py [indir] [method,.ini param]"
  print "ex: python param_quantitative.py ./samples/PETs1 P,coneR1"
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
param_val = -1
for line in open('default.ini'):
  if param in line:
    splitline = line.strip("\n").split(" ")
    param_val = float(splitline[-1])
    print "Init val of " + param + ": " + str(param_val)

print indir + "," + str(paramline)

# ConfigParser impl
config = ConfigParser()
config.optionxform = str  # make sure to not strip capitals
config.read('default.ini')

# init listdir
listing = os.listdir(frames_path)

print "Processing..."
for infile in sorted(listing):

  # use this string to print out values
  readout = ""

  fr = frames_path+infile
  bg = bgs_path+infile
  sh = shadows_path+infile

  # update existing value
  config.set(psection, param, param_val)

  # save to a file
  with open('params.ini', 'w') as configfile:
    config.write(configfile)

  # now that params are set, run SD
  process = subprocess.Popen(["./bin/quantitative", fr, bg, sh], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

  for line in process.stderr:
    splitline = line.strip("\n").split(",")
    if(splitline[0] == section):
      readout += splitline[1]
      readout += ","
      readout += splitline[2]
      readout += ","
      dd = float(splitline[1]) + float(splitline[2])
      readout += str(dd)
      readout += ","

  # we now have original det/discrim values. Calculate new parameter
  model_process = subprocess.Popen(["./bin/avgattenmodel", fr, bg, sh], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

  # gets new value for interrogated param ('ex: coneR1')
  new_val = -1
  for line in model_process.stderr:
    splitline = line.strip("\n").split(",")
    new_val = splitline[0]
    rg_shift = splitline[1]
    readout += new_val
    readout += ","
    readout += str(rg_shift)
    readout += ","

  # update existing value
  config.set(psection, param, float(new_val))

  # save to a file
  with open('params.ini', 'w') as configfile:
    config.write(configfile)

  # now that new param is set, run SD
  process = subprocess.Popen(["./bin/quantitative", fr, bg, sh], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

  for line in process.stderr:
    splitline = line.strip("\n").split(",")
    if(splitline[0] == section):
      readout += splitline[1]
      readout += ","
      readout += splitline[2]
      readout += ","
      dd = float(splitline[1]) + float(splitline[2])
      readout += str(dd)

  print readout
