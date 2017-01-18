#!/bin/bash

BINDIR=bin
RESULTSDIR=results
INDIR=$1

TRIM=${@%/}
TRIM=${TRIM##*/}

echo "Using $1..."

for file in $INDIR/{.,}*;
do
  mkdir -p $RESULTSDIR/$TRIM
  $BINDIR/hsvparams "$file" >> $RESULTSDIR/$TRIM/hsvparams.csv  # HSV
done

echo "Wrote to $RESULTSDIR/$TRIM/hsvparams.csv"
