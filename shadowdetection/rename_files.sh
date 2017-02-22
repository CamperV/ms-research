#!/bin/bash

INDIR=$1
SD=shadows
BG=bgs
FR=frames

EXT=jpg

if [ "$#" -ne 1 ]; then
  echo "Usage: /bin/bash rename_files.sh [samples]"
  exit 1
fi

TRIM=${@%/}
TRIM=${TRIM##*/}

for file in $INDIR/$FR/{.,}*;
do
  if [ $(basename "$file") == "." ] || [ $(basename "$file") == ".." ]; then 
    continue;
  fi

  filename=$(basename "$file") 
  filename_str="${filename%.*}"
  filename_ext="${filename#*.}"
  filename_num="${filename_str##*_}"

  echo "$file  ->  $INDIR/$FR/$filename_num.$EXT"
  mv $file $INDIR/$FR/$filename_num.$EXT

done

for file in $INDIR/$BG/{.,}*;
do
  if [ $(basename "$file") == "." ] || [ $(basename "$file") == ".." ]; then 
    continue;
  fi

  filename=$(basename "$file") 
  filename_str="${filename%.*}"
  filename_ext="${filename#*.}"
  filename_num="${filename_str##*_}"

  echo "$file  ->  $INDIR/$BG/$filename_num.$EXT"
  mv $file $INDIR/$BG/$filename_num.$EXT

done

for file in $INDIR/$SD/{.,}*;
do
  if [ $(basename "$file") == "." ] || [ $(basename "$file") == ".." ]; then 
    continue;
  fi

  filename=$(basename "$file") 
  filename_str="${filename%.*}"
  filename_ext="${filename#*.}"
  filename_num="${filename_str##*_}"

  echo "$file  ->  $INDIR/$SD/$filename_num.$EXT"
  mv $file $INDIR/$SD/$filename_num.$EXT

done
