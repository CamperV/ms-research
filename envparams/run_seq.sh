#!/bin/bash

INDIR=$1

for file in $INDIR/{.,}*;
do
  ./seqparams "$file" >> seqparams.csv  # HSV
done
