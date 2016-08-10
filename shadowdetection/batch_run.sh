#!/bin/bash

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

for file in $INDIR/$FR/{.,}*;
do
  if [ $(basename "$file") == "." ] || [ $(basename "$file") == ".." ]; then 
    continue;
  fi

  filename=$(basename "$file") 
  filename="${filename%.*}"

  #./quantitative $file $INDIR/$BG/$filename.* $INDIR/$SD/$filename.*
  array=(`python run_quantitative.py $file $INDIR/$BG/$filename.* $INDIR/$SD/$filename.*`)
  echo ${array[@]}

  counter=$((++counter))

  cDetect=$(bc <<< "$cDetect+${array[1]}")
  cDiscrim=$(bc <<< "$cDiscrim+${array[2]}")

  pDetect=$(bc <<< "$pDetect+${array[4]}")
  pDiscrim=$(bc <<< "$pDiscrim+${array[5]}")

  gDetect=$(bc <<< "$gDetect+${array[7]}")
  gDiscrim=$(bc <<< "$gDiscrim+${array[8]}")

  srDetect=$(bc <<< "$srDetect+${array[10]}")
  srDiscrim=$(bc <<< "$srDiscrim+${array[11]}")

  lrDetect=$(bc <<< "$lrDetect+${array[13]}")
  lrDiscrim=$(bc <<< "$lrDiscrim+${array[14]}")

done

echo ""
echo -n "C $(bc <<< "scale=4;$cDetect/$counter") $(bc <<< "scale=4;$cDiscrim/$counter") "
echo -n "P $(bc <<< "scale=4;$pDetect/$counter") $(bc <<< "scale=4;$pDiscrim/$counter") "
echo -n "G $(bc <<< "scale=4;$gDetect/$counter") $(bc <<< "scale=4;$gDiscrim/$counter") "
echo -n "S $(bc <<< "scale=4;$srDetect/$counter") $(bc <<< "scale=4;$srDiscrim/$counter") "
echo "L $(bc <<< "scale=4;$lrDetect/$counter") $(bc <<< "scale=4;$lrDiscrim/$counter") "
