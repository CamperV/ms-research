#!/bin/bash
./run_avgatten.sh ../shadowdetection/samples/$1
cat results/$1/avgattenparams.csv
