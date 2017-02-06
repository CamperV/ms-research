#!/bin/bash

BINDIR=bin
RESULTSDIR=results
INDIR=$1

TRIM=${@%/}
TRIM=${TRIM##*/}

[ -e $RESULTSDIR/$TRIM/siftparams.csv ] && rm $RESULTSDIR/$TRIM/siftparams.csv

if [ "$#" -ne 1 ]; then
  echo "Usage: /bin/bash run_sift.sh [samples]"
  exit 1
fi

echo ""
echo "---"
echo "NOTE: Run with envparams/samples!"
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
  $BINDIR/siftparams "$file" >> $RESULTSDIR/$TRIM/siftparams.csv  # SIFT nums
done

echo "Wrote to $RESULTSDIR/$TRIM/siftparams.csv"
