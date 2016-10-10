#!/bin/bash

INDIR=$1

for file in $INDIR/{.,}*;
do
  ./envparams "$file" $INDIR/gndtruth.jpg
done
