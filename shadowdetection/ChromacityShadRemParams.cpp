// Copyright (C) 2011 NICTA (www.nicta.com.au)
// Copyright (C) 2011 Andres Sanin
//
// This file is provided without any warranty of fitness for any purpose.
// You can redistribute this file and/or modify it under the terms of
// the GNU General Public License (GPL) as published by the
// Free Software Foundation, either version 3 of the License
// or (at your option) any later version.
// (see http://www.opensource.org/licenses for more info)

#include <iostream>
#include "ChromacityShadRemParams.h"
#include "SimpleIni.h"

ChromacityShadRemParams::ChromacityShadRemParams() {

  CSimpleIniA ini(true, true, true);
  if (ini.LoadFile("params.ini") < 0) throw "Load failed for file 1";

	winSize = atoi(ini.GetValue("ChromacityShadRemParams", "winSize", NULL));
	cleanSrMask = (bool)atoi(ini.GetValue("ChromacityShadRemParams", "cleanSrMask", NULL));
	hThresh = atoi(ini.GetValue("ChromacityShadRemParams", "hThresh", NULL));
	sThresh = atoi(ini.GetValue("ChromacityShadRemParams", "sThresh", NULL));
	vThreshUpper = atof(ini.GetValue("ChromacityShadRemParams", "vThreshUpper", NULL));
	vThreshLower = atof(ini.GetValue("ChromacityShadRemParams", "vThreshLower", NULL));
}

ChromacityShadRemParams::~ChromacityShadRemParams() {
}
