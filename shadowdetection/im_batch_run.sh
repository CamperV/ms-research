#!/bin/bash

INDIR=$1
SD=shadows
BG=bgs
FR=frames

counter=0

cDetect=0
cDiscrim=0
pDetect=0
pDiscrim=0
gDetect=0
gDiscrim=0
srDetect=0
srDiscrim=0
lrDetect=0
lrDiscrim=0

if [ "$#" -ne 1 ]; then
  echo "Usage: /bin/bash batch_run.sh [samples]"
  exit 1
fi

for file in $INDIR/$FR/{.,}*;
do
  if [ $(basename "$file") == "." ] || [ $(basename "$file") == ".." ]; then 
    continue;
  fi

  filename=$(basename "$file") 
  filename="${filename%.*}"

  ./quantitative2 $file $INDIR/$BG/$filename.* $INDIR/$SD/$filename.*

done
