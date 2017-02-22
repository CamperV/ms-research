#!/bin/bash
echo "PETs1 [1/8] [==>             ]"
python param_quantitative.py ./samples/PETs1/ P,postThresh 0.3,0.001 optimal >> overnight_results_
echo "PETs2 [2/8] [====>           ]"
python param_quantitative.py ./samples/PETs2/ P,postThresh 0.3,0.001 optimal >> overnight_results_postThresh
echo "aton_campus [3/8] [======>         ]"
python param_quantitative.py ./samples/aton_campus/ P,postThresh 0.3,0.001 optimal >> overnight_results_postThresh
echo "aton_hallway [4/8] [========>       ]"
python param_quantitative.py ./samples/aton_hallway/ P,postThresh 0.3,0.001 optimal >> overnight_results_postThresh
echo "aton_highway1 [5/8] [==========>     ]"
python param_quantitative.py ./samples/aton_highway1/ P,postThresh 0.3,0.001 optimal >> overnight_results_postThresh
echo "aton_highway3 [6/8] [============>   ]"
python param_quantitative.py ./samples/aton_highway3/ P,postThresh 0.3,0.001 optimal >> overnight_results_postThresh
echo "aton_lab [7/8] [==============> ]"
python param_quantitative.py ./samples/aton_lab/ P,postThresh 0.3,0.001 optimal >> overnight_results_postThresh
echo "aton_room [8/8] [================]"
python param_quantitative.py ./samples/aton_room/ P,postThresh 0.3,0.001 optimal >> overnight_results_postThresh
