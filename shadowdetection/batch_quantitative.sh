#!/bin/bash

INDIR=$1
S=samples
SD=shadows
BG=bgs
FR=frames

for file in $S/$INDIR/$FR/{.,}*;
do
  if [ $(basename "$file") == "." ] || [ $(basename "$file") == ".." ]; then 
    continue;
  fi


  filename=$(basename "$file") 
  filename="${filename%.*}"

  #./quantitative $file $S/$INDIR/$BG/$filename.jpg $S/$INDIR/$SD/$filename.png
  ./quantitative $file $S/$INDIR/$BG/$filename.* $S/$INDIR/$SD/$filename.*
done
