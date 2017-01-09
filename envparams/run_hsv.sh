#!/bin/bash

INDIR=$1

for file in $INDIR/{.,}*;
do
  ./hsvparams "$file" >> hsvparams.csv  # HSV
done
