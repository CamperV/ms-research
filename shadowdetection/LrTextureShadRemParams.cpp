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
#include "LrTextureShadRemParams.h"
#include "SimpleIni.h"

LrTextureShadRemParams::LrTextureShadRemParams() {
  CSimpleIniA ini(true, true, true);
  if (ini.LoadFile("params.ini") < 0) throw "Load failed for file 1";

	avgSatThresh = atoi(ini.GetValue("LrTextureShadRemParams", "avgSatThresh", NULL));
	hThreshLowSat = atoi(ini.GetValue("LrTextureShadRemParams", "hThreshLowSat", NULL));
	hThreshHighSat = atoi(ini.GetValue("LrTextureShadRemParams", "hThreshHighSat", NULL));
	sThreshLowSat = atoi(ini.GetValue("LrTextureShadRemParams", "sThreshLowSat", NULL));
	sThreshHighSat = atoi(ini.GetValue("LrTextureShadRemParams", "sThreshHighSat", NULL));
	avgAttenThresh = atof(ini.GetValue("LrTextureShadRemParams", "avgAttenThresh", NULL));
	vThreshUpperLowAtten = atof(ini.GetValue("LrTextureShadRemParams", "vThreshUpperLowAtten", NULL));
	vThreshUpperHighAtten = atof(ini.GetValue("LrTextureShadRemParams", "vThreshUpperHighAtten", NULL));
	vThreshLowerLowAtten = atof(ini.GetValue("LrTextureShadRemParams", "vThreshLowerLowAtten", NULL));
	vThreshLowerHighAtten = atof(ini.GetValue("LrTextureShadRemParams", "vThreshLowerHighAtten", NULL));
	avgPerimThresh = atoi(ini.GetValue("LrTextureShadRemParams", "avgPerimThresh", NULL));
	edgeDiffRadius = atoi(ini.GetValue("LrTextureShadRemParams", "edgeDiffRadius", NULL));
	borderDiffRadius = atoi(ini.GetValue("LrTextureShadRemParams", "borderDiffRadius", NULL));
	splitIncrement = atoi(ini.GetValue("LrTextureShadRemParams", "splitIncrement", NULL));
	splitRadius = atoi(ini.GetValue("LrTextureShadRemParams", "splitRadius", NULL));

	cannyThresh1 = atoi(ini.GetValue("LrTextureShadRemParams", "cannyThresh1", NULL));
	cannyThresh2 = atoi(ini.GetValue("LrTextureShadRemParams", "cannyThresh2", NULL));
	cannyApertureSize = atoi(ini.GetValue("LrTextureShadRemParams", "cannyApertureSize", NULL));
	cannyL2Grad = (bool)atoi(ini.GetValue("LrTextureShadRemParams", "cannyL2Grad", NULL));

	minCorrPoints = atoi(ini.GetValue("LrTextureShadRemParams", "minCorrPoints", NULL));
	maxCorrRounds = atoi(ini.GetValue("LrTextureShadRemParams", "maxCorrRounds", NULL));
	corrBorder = atoi(ini.GetValue("LrTextureShadRemParams", "corrBorder", NULL));
	gradScales = atoi(ini.GetValue("LrTextureShadRemParams", "gradScales", NULL));
	gradMagThresh = atoi(ini.GetValue("LrTextureShadRemParams", "gradMagThresh", NULL));
	gradAttenThresh = atof(ini.GetValue("LrTextureShadRemParams", "gradAttenThresh", NULL));
	gradDistThresh = atof(ini.GetValue("LrTextureShadRemParams", "gradDistThresh", NULL));
	gradCorrThreshLowAtten = atof(ini.GetValue("LrTextureShadRemParams", "gradCorrThreshLowAtten", NULL));
	gradCorrThreshHighAtten = atof(ini.GetValue("LrTextureShadRemParams", "gradCorrThreshHighAtten", NULL));

	cleanShadows = (bool)atoi(ini.GetValue("LrTextureShadRemParams", "cleanShadows", NULL));
	fillShadows = (bool)atoi(ini.GetValue("LrTextureShadRemParams", "fillShadows", NULL));
	minShadowPerim = atoi(ini.GetValue("LrTextureShadRemParams", "minShadowPerim", NULL));
	cleanSrMask = (bool)atoi(ini.GetValue("LrTextureShadRemParams", "cleanSrMask", NULL));
	fillSrMask = (bool)atoi(ini.GetValue("LrTextureShadRemParams", "fillSrMask", NULL));
}

LrTextureShadRemParams::~LrTextureShadRemParams() {
}
