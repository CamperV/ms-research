// Copyright (C) 2011 NICTA (www.nicta.com.au)
// Copyright (C) 2011 Andres Sanin
//
// This file is provided without any warranty of fitness for any purpose.
// You can redistribute this file and/or modify it under the terms of
// the GNU General Public License (GPL) as published by the
// Free Software Foundation, either version 3 of the License
// or (at your option) any later version.
// (see http://www.opensource.org/licenses for more info)
//
// ---
// Jay Danner
// ---

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/video.hpp"
#include <opencv2/video/background_segm.hpp>

#include "ChromacityShadRem.h"
#include "GeometryShadRem.h"
#include "LrTextureShadRem.h"
#include "PhysicalShadRem.h"
#include "SrTextureShadRem.h"
#include "utils/VideoStats.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
  
  if(argc > 1) {
    cout << "Opening video capture " << argv[1] << "..." << endl;
  } else {
    cout << "Enter video filename." << endl;
    exit(0);
  }

  VideoCapture capture(argv[1]);
  VideoStats vstats;

  vstats.setWidth(capture.get(CV_CAP_PROP_FRAME_WIDTH));
  vstats.setHeight(capture.get(CV_CAP_PROP_FRAME_HEIGHT));

  if (!capture.isOpened()) { 
    cout << "Capture failed to open." << endl; 
    return -1; 
  }

  Mat frame, fg, bg;
  capture >> frame;

	// create shadow removers
	ChromacityShadRem chr;
	SrTextureShadRem srTex;
	LrTextureShadRem lrTex;

	// matrices to store the masks after shadow removal
	Mat chrMask, srTexMask, lrTexMask;

  // init windows
  namedWindow("Chromacity", 1);
  //namedWindow("Small Region Texture", 1);
  namedWindow("Large Region Texture", 1);

  /* TRACKBARS & PARAMS */

  // Chromacity
  int vThreshUpperInt = 100;
  int vThreshLowerInt = 30;

  createTrackbar("hThresh", "Chromacity", &chr.params.hThresh, 255);
  createTrackbar("sThresh", "Chromacity", &chr.params.sThresh, 255);
  createTrackbar("vThreshUpper", "Chromacity", &vThreshUpperInt, 100);
  createTrackbar("vThreshLower", "Chromacity", &vThreshLowerInt, 100);

  // Large Region
  int avgSatThreshInt = 35;
  int avgAttenThreshInt = 158;
  int vThreshUpperLowAttenInt = 100;
  int vThreshUpperHighAttenInt = 99;
  int vThreshLowerLowAttenInt = 60;
  int vThreshLowerHighAttenInt = 21;

  int avgPerimThreshInt = 100;
  int cannyThresh1Int = 72;
  int cannyThresh2Int = 94;

  int gradMagThreshInt = 6;
  int gradAttenThreshInt = 10;
  int gradDistThreshInt = CV_PI * 10;
  int gradCorrThreshLowAttenInt = 20;
  int gradCorrThreshHighAttenInt = 10;

  createTrackbar("avgSatThresh", "Large Region Texture", &avgSatThreshInt, 255);
  createTrackbar("hThreshLowSat", "Large Region Texture", &lrTex.params.hThreshLowSat, 255);
  createTrackbar("hThreshHighSat", "Large Region Texture", &lrTex.params.hThreshHighSat, 255);
  createTrackbar("sThreshLowSat", "Large Region Texture", &lrTex.params.sThreshLowSat, 255);
  createTrackbar("sThreshHighSat", "Large Region Texture", &lrTex.params.sThreshHighSat, 255);

  createTrackbar("avgAttenThresh", "Large Region Texture", &avgAttenThreshInt, 500);
  createTrackbar("vThreshUpperLowAtten", "Large Region Texture", &vThreshUpperLowAttenInt, 100);
  createTrackbar("vThreshUpperHighAtten", "Large Region Texture", &vThreshUpperHighAttenInt, 100);
  createTrackbar("vThreshLowerLowAtten", "Large Region Texture", &vThreshLowerLowAttenInt, 100);
  createTrackbar("vThreshLowerHighAtten", "Large Region Texture", &vThreshLowerHighAttenInt, 100);

  //createTrackbar("avgPerimThresh", "Large Region Texture", &avgPerimThreshInt, 500);
  //createTrackbar("edgeDiffRadius", "Large Region Texture", &lrTex.params.edgeDiffRadius, 10);
  //createTrackbar("borderDiffRadius", "Large Region Texture", &lrTex.params.borderDiffRadius, 10);
  //createTrackbar("splitIncrement", "Large Region Texture", &lrTex.params.splitIncrement, 10);
  //createTrackbar("splitRadius", "Large Region Texture", &lrTex.params.splitRadius, 10);

  createTrackbar("cannyThresh1", "Large Region Texture", &cannyThresh1Int, 200);
  createTrackbar("cannyThresh2", "Large Region Texture", &cannyThresh2Int, 200);
  createTrackbar("cannyApertureSize", "Large Region Texture", &lrTex.params.cannyApertureSize, 10);

  //createTrackbar("minCorrPoints", "Large Region Texture", &lrTex.params.minCorrPoints, 10);
  //createTrackbar("maxCorrRounds", "Large Region Texture", &lrTex.params.maxCorrRounds, 10);
  //createTrackbar("corrBorder", "Large Region Texture", &lrTex.params.corrBorder, 10);

  createTrackbar("gradScales", "Large Region Texture", &lrTex.params.gradScales, 10);
  createTrackbar("gradMagThresh", "Large Region Texture", &gradMagThreshInt, 10);
  createTrackbar("gradAttenThresh", "Large Region Texture", &gradAttenThreshInt, 100);
  createTrackbar("gradDistThresh", "Large Region Texture", &gradDistThreshInt, 100);
  createTrackbar("gradCorrThreshLowAtten", "Large Region Texture", &gradCorrThreshLowAttenInt, 100);
  createTrackbar("gradCorrThreshHighAtten", "Large Region Texture", &gradCorrThreshHighAttenInt, 100);

  BackgroundSubtractorMOG2 MOG = BackgroundSubtractorMOG2();
  MOG.set("detectShadows", false);
  MOG.set("nmixtures", 3);

  bool step = false;

  // processing loop
  for(;;) {

    /* PRE-PROCESSING */

    // UPDATE FLOAT PARAMS
    chr.params.vThreshUpper = (float)vThreshUpperInt / 100.0;
    chr.params.vThreshLower = (float)vThreshLowerInt / 100.0;

    lrTex.params.avgSatThresh = (float)avgSatThreshInt;
    lrTex.params.avgAttenThresh = (float)avgAttenThreshInt / 100.0;
    lrTex.params.vThreshUpperLowAtten = (float)vThreshUpperLowAttenInt / 100.0;
    lrTex.params.vThreshUpperHighAtten = (float)vThreshUpperHighAttenInt / 100.0;
    lrTex.params.vThreshLowerLowAtten = (float)vThreshLowerLowAttenInt / 100.0;
    lrTex.params.vThreshLowerHighAtten = (float)vThreshLowerHighAttenInt / 100.0;
    lrTex.params.avgPerimThresh = (float)avgPerimThreshInt;
    lrTex.params.cannyThresh1 = (float)cannyThresh1Int;
    lrTex.params.cannyThresh2 = (float)cannyThresh2Int;
    lrTex.params.gradMagThresh = (float)gradMagThreshInt;
    lrTex.params.gradAttenThresh = (float)gradAttenThreshInt / 100.0;
    lrTex.params.gradDistThresh = (float)gradDistThreshInt / 100.0;
    lrTex.params.gradCorrThreshLowAtten = (float)gradCorrThreshLowAttenInt / 100.0;
    lrTex.params.gradCorrThreshHighAtten = (float)gradCorrThreshHighAttenInt / 100.0;

    capture >> frame;

    // Looks like we've hit the end of our feed! Restart
    if(frame.empty()) {
        cout << "Frame is empty, restarting..." << endl;
        capture.set(CV_CAP_PROP_POS_AVI_RATIO, 0.0);
        cout << "Done!" << endl;
        continue;
    }

    MOG(frame, fg);
    MOG.getBackgroundImage(bg);

    /* PROCESSING */

#ifdef PRECALC
    Mat shadowMask = Mat::zeros(frame.rows, frame.cols, CV_8U);

	  chr.removeShadows(frame, fg, bg, chrMask, shadowMask);
	  srTex.removeShadows(frame, fg, bg, srTexMask, shadowMask);
	  lrTex.removeShadows(frame, fg, bg, lrTexMask, shadowMask);

#else
	  chr.removeShadows(frame, fg, bg, chrMask);
	  //srTex.removeShadows(frame, fg, bg, srTexMask);
	  lrTex.removeShadows(frame, fg, bg, lrTexMask);

#endif

    /* POST-PROCESSING */

    vstats.updateFPS();
    vstats.displayStats();

	  // show results
	  imshow("Frame", frame);                       // og frame
	  //imshow("Foreground", fg);                   // foreground
	  imshow("Chromacity", chrMask);              // chromacity-detected shadows
	  //imshow("Small Region Texture", srTexMask);  // small-region shadows
	  imshow("Large Region Texture", lrTexMask);    // large-region shadows

    /* STEP-THROUGH */

    if(step) {
      if(waitKey(0) == 115) step = !step;
    } else {
      if(waitKey(30) == 115) step = !step;
    }
  }
	return 0;
}
