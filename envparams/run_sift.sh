#!/bin/bash

BINDIR=bin
RESULTSDIR=results
INDIR=$1

TRIM=${@%/}
TRIM=${TRIM##*/}

for file in $INDIR/{.,}*;
do
  mkdir -p $RESULTSDIR/$TRIM
  $BINDIR/siftparams "$file" >> $RESULTSDIR/$TRIM/siftparams.csv  # SIFT nums
done
