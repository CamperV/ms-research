#!/bin/bash
echo "PETs1 [1/8] [==>             ]"
python param_quantitative.py ./samples/PETs1/ P,weightSmootTerm 20,1 optimal >> overnight_results_weightSmootTerm
echo "PETs2 [2/8] [====>           ]"
python param_quantitative.py ./samples/PETs2/ P,weightSmootTerm 20,1 optimal >> overnight_results_weightSmootTerm
echo "aton_campus [3/8] [======>         ]"
python param_quantitative.py ./samples/aton_campus/ P,weightSmootTerm 20,1 optimal >> overnight_results_weightSmootTerm
echo "aton_hallway [4/8] [========>       ]"
python param_quantitative.py ./samples/aton_hallway/ P,weightSmootTerm 20,1 optimal >> overnight_results_weightSmootTerm
echo "aton_highway1 [5/8] [==========>     ]"
python param_quantitative.py ./samples/aton_highway1/ P,weightSmootTerm 20,1 optimal >> overnight_results_weightSmootTerm
echo "aton_highway3 [6/8] [============>   ]"
python param_quantitative.py ./samples/aton_highway3/ P,weightSmootTerm 20,1 optimal >> overnight_results_weightSmootTerm
echo "aton_lab [7/8] [==============> ]"
python param_quantitative.py ./samples/aton_lab/ P,weightSmootTerm 20,1 optimal >> overnight_results_weightSmootTerm
echo "aton_room [8/8] [================]"
python param_quantitative.py ./samples/aton_room/ P,weightSmootTerm 20,1 optimal >> overnight_results_weightSmootTerm
