# a python script for running and collecting the outputs of C++ program 'quantitative.'

import sys
import subprocess

try:
  filename = sys.argv[1]
  bgs = sys.argv[2]
  shadows = sys.argv[3]
except:
  print "Please enter arguments"
  sys.exit(0)

cDetect = 0
cDiscrim = 0
pDetect = 0
pDiscrim = 0
gDetect = 0
gDiscrim = 0
srDetect = 0
srDiscrim = 0
lrDetect = 0
lrDiscrim = 0

process = subprocess.Popen(["./bin/quantitative", filename, bgs, shadows], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

for line in process.stderr:
  splitline = line.strip("\n").split(",")

  #print splitline[0] + "," + splitline[1] + ";" + splitline[2]
  print splitline[1] + " " + splitline[2]

  if(splitline[0] == "C"):
    cDetect += float(splitline[1])
    cDiscrim += float(splitline[2])
  elif(splitline[0] == "P"):
    pDetect += float(splitline[1])
    pDiscrim += float(splitline[2])
  elif(splitline[0] == "G"):
    gDetect += float(splitline[1])
    gDiscrim += float(splitline[2])
  elif(splitline[0] == "SR"):
    srDetect += float(splitline[1])
    srDiscrim += float(splitline[2])
  elif(splitline[0] == "LR"):
    lrDetect += float(splitline[1])
    lrDiscrim += float(splitline[2])

