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

[ -e $RESULTSDIR/$TRIM/colorsiftparams.csv ] && rm $RESULTSDIR/$TRIM/colorsiftparams.csv

if [ "$#" -ne 1 ]; then
  echo "Usage: /bin/bash run_colorsift.sh [samples]"
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
  $BINDIR/colorsiftparams $file $INDIR/$BG/$filename.* 0 >> $RESULTSDIR/$TRIM/colorsiftparams.csv
done

echo "Wrote to $RESULTSDIR/$TRIM/colorsiftparams.csv"
