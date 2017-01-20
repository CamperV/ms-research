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
  $BINDIR/bowparams "$file" >> $RESULTSDIR/$TRIM/bowparams.csv  # BOW analytics
done

echo "Wrote to $RESULTSDIR/$TRIM/bowparams.csv"
