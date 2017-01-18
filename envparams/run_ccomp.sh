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
  $BINDIR/ccompparams "$file" >> $RESULTSDIR/$TRIM/ccompparams.csv  # avg ccomp perim, # of ccomps
done

echo "Wrote to $RESULTSDIR/$TRIM/ccompparams.csv"
