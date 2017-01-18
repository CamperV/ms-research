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

if [ "$#" -ne 1 ]; then
  echo "Usage: /bin/bash run_avgatten.sh [samples]"
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

  filename=$(basename "$file") 
  filename="${filename%.*}"

  mkdir -p $RESULTSDIR/$TRIM
  $BINDIR/avgattenparams $file $INDIR/$BG/$filename.* $INDIR/$SD/$filename.* >> $RESULTSDIR/$TRIM/avgattenparams.csv
done

echo "Wrote to $RESULTSDIR/$TRIM/avgattenparams.csv"
