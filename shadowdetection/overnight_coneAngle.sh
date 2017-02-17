#!/bin/bash
echo "PETs1 [1/8] [==>             ]"
#python param_quantitative.py ./samples/PETs1/ P,coneAngle 0.3,0.0005 optimal >> overnight_results_coneAngle0005
echo "PETs2 [2/8] [====>           ]"
#python param_quantitative.py ./samples/PETs2/ P,coneAngle 0.3,0.0005 optimal >> overnight_results_coneAngle0005
echo "aton_campus [3/8] [======>         ]"
python param_quantitative.py ./samples/aton_campus/ P,coneAngle 0.3,0.0005 optimal >> overnight_results_coneAngle0005
echo "aton_hallway [4/8] [========>       ]"
python param_quantitative.py ./samples/aton_hallway/ P,coneAngle 0.3,0.0005 optimal >> overnight_results_coneAngle0005
echo "aton_highway1 [5/8] [==========>     ]"
python param_quantitative.py ./samples/aton_highway1/ P,coneAngle 0.3,0.0005 optimal >> overnight_results_coneAngle0005
echo "aton_highway3 [6/8] [============>   ]"
python param_quantitative.py ./samples/aton_highway3/ P,coneAngle 0.3,0.0005 optimal >> overnight_results_coneAngle0005
echo "aton_lab [7/8] [==============> ]"
python param_quantitative.py ./samples/aton_lab/ P,coneAngle 0.3,0.0005 optimal >> overnight_results_coneAngle0005
echo "aton_room [8/8] [================]"
python param_quantitative.py ./samples/aton_room/ P,coneAngle 0.3,0.0005 optimal >> overnight_results_coneAngle0005
