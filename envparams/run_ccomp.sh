#!/bin/bash

BINDIR=bin
RESULTSDIR=results
INDIR=$1

TRIM=${@%/}
TRIM=${TRIM##*/}

[ -e $RESULTSDIR/$TRIM/ccompparams.csv ] && rm $RESULTSDIR/$TRIM/ccompparams.csv

if [ "$#" -ne 1 ]; then
  echo "Usage: /bin/bash run_ccomp.sh [samples/fg]"
  exit 1
fi

echo ""
echo "---"
echo "NOTE: Run with shadowdetection/samples/fg [shadows]!"
echo "---"
echo ""
echo "Using $1..."

for file in $INDIR/{.,}*;
do
  if [ $(basename "$file") == "." ] || [ $(basename "$file") == ".." ]; then 
    continue;
  fi

  if [ $(basename "$file") == "fg" ] || [ $(basename "$file") == "bgs" ]; then 
    continue;
  fi

  mkdir -p $RESULTSDIR/$TRIM
  $BINDIR/ccompparams "$file" >> $RESULTSDIR/$TRIM/ccompparams.csv  # avg ccomp perim, # of ccomps
done

echo "Wrote to $RESULTSDIR/$TRIM/ccompparams.csv"
