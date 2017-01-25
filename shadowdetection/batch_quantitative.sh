#!/bin/bash

RESULTSDIR=results
INDIR=$1
SD=shadows
BG=bgs
FR=frames

counter=0

cDetect=0
cDiscrim=0
pDetect=0
pDiscrim=0
gDetect=0
gDiscrim=0
srDetect=0
srDiscrim=0
lrDetect=0
lrDiscrim=0

if [ "$#" -ne 1 ]; then
  echo "Usage: /bin/bash batch_run.sh [samples]"
  exit 1
fi

TRIM=${@%/}
TRIM=${TRIM##*/}

[ -e $RESULTSDIR/$TRIM/shadowdetection.csv ] && rm $RESULTSDIR/$TRIM/shadowdetection.csv

echo ""
echo "---"
echo "NOTE: Run with shadowdetection/samples!"
echo "---"
echo ""
echo "---"
echo "NOTE II - The Quickening: Order in CSV is: Chromacity (C), Physical (P), Geometry (G), Small Region (SR), Large Region (LR)"
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
  #array=(`python run_quantitative.py $file $INDIR/$BG/$filename.* $INDIR/$SD/$filename.*`)
  array=($(python run_quantitative.py $file $INDIR/$BG/$filename.* $INDIR/$SD/$filename.*))
  echo ${array[@]} >> $RESULTSDIR/$TRIM/shadowdetection.csv

  counter=$((++counter))

  cDetect=$(bc <<< "$cDetect+${array[0]}")
  cDiscrim=$(bc <<< "$cDiscrim+${array[1]}")

  pDetect=$(bc <<< "$pDetect+${array[2]}")
  pDiscrim=$(bc <<< "$pDiscrim+${array[3]}")

  gDetect=$(bc <<< "$gDetect+${array[4]}")
  gDiscrim=$(bc <<< "$gDiscrim+${array[5]}")

  srDetect=$(bc <<< "$srDetect+${array[6]}")
  srDiscrim=$(bc <<< "$srDiscrim+${array[7]}")

  lrDetect=$(bc <<< "$lrDetect+${array[8]}")
  lrDiscrim=$(bc <<< "$lrDiscrim+${array[9]}")

done

echo ""
echo "Averages:"
echo -n "C $(bc <<< "scale=4;$cDetect/$counter") $(bc <<< "scale=4;$cDiscrim/$counter") "
echo -n "P $(bc <<< "scale=4;$pDetect/$counter") $(bc <<< "scale=4;$pDiscrim/$counter") "
echo -n "G $(bc <<< "scale=4;$gDetect/$counter") $(bc <<< "scale=4;$gDiscrim/$counter") "
echo -n "S $(bc <<< "scale=4;$srDetect/$counter") $(bc <<< "scale=4;$srDiscrim/$counter") "
echo "L $(bc <<< "scale=4;$lrDetect/$counter") $(bc <<< "scale=4;$lrDiscrim/$counter") "
