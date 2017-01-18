#!/bin/bash

BINDIR=bin
RESULTSDIR=results
INDIR=$1

TRIM=${@%/}
TRIM=${TRIM##*/}

if [ "$#" -ne 1 ]; then
  echo "Usage: /bin/bash run_gist.sh [samples]"
  exit 1
fi

echo ""
echo "---"
echo "NOTE: Be mindful of what GIST parameters you are looking for. Otherwise, information overload."
echo "---"
echo ""
echo ""
echo "---"
echo "NOTE II - The Quickening: gistparams is a symlink to a binary made (via cmake). Make sure to build C++ wrapper LibGIST first. (https://github.com/tgsong/LibGIST)"
echo "---"
echo ""
echo "Using $1..."

for file in $INDIR/{.,}*;
do
  mkdir -p $RESULTSDIR/$TRIM
  $BINDIR/gistparams "$file" >> $RESULTSDIR/$TRIM/gistparams.csv  # GIST params
done

echo "Wrote to $RESULTSDIR/$TRIM/gistparams.csv"
