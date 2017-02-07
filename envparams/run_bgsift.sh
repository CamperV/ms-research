#!/bin/bash

INDIR=$1
SD=shadows
BG=bgs
FR=frames

BINDIR=bin
RESULTSDIR=results
INDIR=$1

TRIM=${@%/}
TRIM=${TRIM##*/}

[ -e $RESULTSDIR/$TRIM/bgsiftparams.csv ] && rm $RESULTSDIR/$TRIM/bgsiftparams.csv

if [ "$#" -ne 1 ]; then
  echo "Usage: /bin/bash run_bgsift.sh [samples]"
  exit 1
fi

echo ""
echo "---"
echo "NOTE: Run with shadowdetection/samples [frames/shadows/bgs]!"
echo "---"
echo ""
echo "Using $1..."

for file in $INDIR/$FR/{.,}*;
do
  if [ $(basename "$file") == "." ] || [ $(basename "$file") == ".." ]; then
    continue;
  fi

  if [ $(basename "$file") == "fg" ] || [ $(basename "$file") == "bgs" ]; then
    continue;
  fi

  filename=$(basename "$file")
  filename="${filename%.*}"

  mkdir -p $RESULTSDIR/$TRIM
  $BINDIR/bgsiftparams $file $INDIR/$BG/$filename.* 0 >> $RESULTSDIR/$TRIM/bgsiftparams.csv
done

echo "Wrote to $RESULTSDIR/$TRIM/bgsiftparams.csv"
