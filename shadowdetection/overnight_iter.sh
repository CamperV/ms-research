#!/bin/bash
echo "PETs1 [1/8] [==>             ]"
#python param_iter.py ./samples/PETs1/ P,coneR1 1.0,0.001
echo "PETs2 [2/8] [====>           ]"
#python param_iter.py ./samples/PETs2/ P,coneR1 1.0,0.001
echo "aton_campus [3/8] [======>         ]"
#python param_iter.py ./samples/aton_campus/ P,coneR1 1.0,0.001
echo "aton_hallway [4/8] [========>       ]"
#python param_iter.py ./samples/aton_hallway/ P,coneR1 1.0,0.001
echo "aton_highway1 [5/8] [==========>     ]"
#python param_iter.py ./samples/aton_highway1/ P,coneR1 1.0,0.001
echo "aton_highway3 [6/8] [============>   ]"
#python param_iter.py ./samples/aton_highway3/ P,coneR1 1.0,0.001
echo "aton_lab [7/8] [==============> ]"
#python param_iter.py ./samples/aton_lab/ P,coneR1 1.0,0.001
echo "aton_room [8/8] [================]"
python param_iter.py ./samples/aton_room/ P,coneR1 1.0,0.001
