#!/bin/bash

INDIR=$1

for file in $INDIR/{.,}*;
do
  ./ccompparams "$file" >> ccompparams.csv  # avg ccomp perim, # of ccomps
done
