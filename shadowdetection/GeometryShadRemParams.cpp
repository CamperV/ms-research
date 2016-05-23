// Copyright (C) 2011 NICTA (www.nicta.com.au)
// Copyright (C) 2011 Andres Sanin
//
// This file is provided without any warranty of fitness for any purpose.
// You can redistribute this file and/or modify it under the terms of
// the GNU General Public License (GPL) as published by the
// Free Software Foundation, either version 3 of the License
// or (at your option) any later version.
// (see http://www.opensource.org/licenses for more info)

#include "GeometryShadRemParams.h"
#include "SimpleIni.h"

GeometryShadRemParams::GeometryShadRemParams() {
  CSimpleIniA ini(true, true, true);
  if (ini.LoadFile("params.ini") < 0) throw "Load failed for file 1";

	smoothFactor = atoi(ini.GetValue("GeometryShadRemParams", "smoothFactor", NULL));
	headThreshRatio = atoi(ini.GetValue("GeometryShadRemParams", "headThreshRatio", NULL));
	minHeadSeq = atoi(ini.GetValue("GeometryShadRemParams", "minHeadSeq", NULL));
	maxEdgeDistance = atoi(ini.GetValue("GeometryShadRemParams", "maxEdgeDistance", NULL));
	edgeThreshRatio = atoi(ini.GetValue("GeometryShadRemParams", "edgeThreshRatio", NULL));
	minEdgeSeq = atoi(ini.GetValue("GeometryShadRemParams", "minEdgeSeq", NULL));
	bottomShiftRatio = atoi(ini.GetValue("GeometryShadRemParams", "bottomShiftRatio", NULL));
	gWeight = atof(ini.GetValue("GeometryShadRemParams", "gWeight", NULL));
	sRelativeWeight = atof(ini.GetValue("GeometryShadRemParams", "sRelativeWeight", NULL));
	thresholdScale = atof(ini.GetValue("GeometryShadRemParams", "thresholdScale", NULL));

	cleanSrMask = (bool)atoi(ini.GetValue("GeometryShadRemParams", "cleanSrMask", NULL));
}

GeometryShadRemParams::~GeometryShadRemParams() {
}
