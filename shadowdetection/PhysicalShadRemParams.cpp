// Copyright (C) 2011 NICTA (www.nicta.com.au)
// Copyright (C) 2011 Andres Sanin
//
// This file is provided without any warranty of fitness for any purpose.
// You can redistribute this file and/or modify it under the terms of
// the GNU General Public License (GPL) as published by the
// Free Software Foundation, either version 3 of the License
// or (at your option) any later version.
// (see http://www.opensource.org/licenses for more info)

#include <cxcore.h>
#include "PhysicalShadRemParams.h"
#include "SimpleIni.h"

PhysicalShadRemParams::PhysicalShadRemParams() {
  CSimpleIniA ini(true, true, true);
  if (ini.LoadFile("params.ini") < 0) throw "Load failed for file 1";

	coneAngle = atof(ini.GetValue("PhysicalShadRemParams", "coneAngle", NULL));
	coneR1 = atof(ini.GetValue("PhysicalShadRemParams", "coneR1", NULL));
	coneR2 = atof(ini.GetValue("PhysicalShadRemParams", "coneR2", NULL));
	weightSmootTerm = atoi(ini.GetValue("PhysicalShadRemParams", "weightSmootTerm", NULL));
	learnBorders = (bool)atoi(ini.GetValue("PhysicalShadRemParams", "learnBorders", NULL));
	gmmGaussians = atoi(ini.GetValue("PhysicalShadRemParams", "gmmGaussians", NULL));
	gmmInitVar = atoi(ini.GetValue("PhysicalShadRemParams", "gmmInitVar", NULL));
	gmmMinVar = atoi(ini.GetValue("PhysicalShadRemParams", "gmmMinVar", NULL));
	gmmStdThreshold = atoi(ini.GetValue("PhysicalShadRemParams", "gmmStdThreshold", NULL));
	gmmWinnerTakesAll = (bool)atoi(ini.GetValue("PhysicalShadRemParams", "gmmWinnerTakesAll", NULL));
	gmmLearningRate = atof(ini.GetValue("PhysicalShadRemParams", "gmmLearningRate", NULL));

	gmmSortMode = GaussianMixtureModel::SORT_BY_WEIGHT;

	gmmFitLogistic = (bool)atoi(ini.GetValue("PhysicalShadRemParams", "gmmFitLogistic", NULL));
	gmmAccumWeightThresh = atof(ini.GetValue("PhysicalShadRemParams", "gmmAccumWeightThresh", NULL));
	postThresh = atof(ini.GetValue("PhysicalShadRemParams", "postThresh", NULL));

	cleanShadows = (bool)atoi(ini.GetValue("PhysicalShadRemParams", "cleanShadows", NULL));
	dilateShadows = (bool)atoi(ini.GetValue("PhysicalShadRemParams", "dilateShadows", NULL));
	cleanSrMask = (bool)atoi(ini.GetValue("PhysicalShadRemParams", "cleanSrMask", NULL));
}

PhysicalShadRemParams::~PhysicalShadRemParams() {
}
