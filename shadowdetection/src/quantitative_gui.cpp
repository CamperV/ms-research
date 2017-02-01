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
#include <sstream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/video/background_segm.hpp"

#include "ChromacityShadRem.h"
#include "GeometryShadRem.h"
#include "LrTextureShadRem.h"
#include "PhysicalShadRem.h"
#include "SrTextureShadRem.h"

#include "VideoStats.h"
#include "Metrics.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
  
  string infile = "";
  if(argc > 3) {
    cout << "\n---" << endl;
    cout << "> Opening image " << argv[1] << "..." << endl;
    cout << "> Opening background " << argv[2] << "..." << endl;
    cout << "> Opening shadows " << argv[3] << "..." << endl;
  } else {
    cout << "Not enough inputs specified: exiting" << endl;
    cout << "Usage: ./singletest [frame] [background] [ground truth]" << endl;
    exit(0);
  }

  Mat frame = imread(argv[1]);
  Mat bg = imread(argv[2]);
  Mat shadows = imread(argv[3], CV_LOAD_IMAGE_GRAYSCALE);

  if (!frame.data) { 
    cout << "Capture failed to open." << endl; 
    return -1; 
  }

  Mat fg;
  threshold(shadows, fg, 0, 255, THRESH_BINARY);
  /*
  absdiff(frame, bg, fg);
  cvtColor(fg, fg, CV_BGR2GRAY);

  // clean up shadows
  threshold(fg, fg, 25, 255, THRESH_BINARY);

  erode(fg, fg, Mat(), Point(-1,-1)); 
  dilate(fg, fg, Mat(), Point(-1,-1)); 
  */

	// create shadow removers
	ChromacityShadRem chr;
  PhysicalShadRem phys;
  GeometryShadRem geo;
	SrTextureShadRem srTex;
	LrTextureShadRem lrTex;

	// matrices to store the masks after shadow removal
	Mat chrMask, physMask, geoMask, srTexMask, lrTexMask;

  // init windows
  //namedWindow("Chromacity", CV_WINDOW_NORMAL);
  namedWindow("Physical", CV_WINDOW_NORMAL);
  //namedWindow("Geometry", CV_WINDOW_NORMAL);
  //namedWindow("Large Region Texture", CV_WINDOW_NORMAL);

  /* TRACKBARS & PARAMS */

  // Chromacity
  int vThreshUpperInt = 100;
  int vThreshLowerInt = 30;

  createTrackbar("hThresh", "Chromacity", &chr.params.hThresh, 255);
  createTrackbar("sThresh", "Chromacity", &chr.params.sThresh, 255);
  createTrackbar("vThreshUpper", "Chromacity", &vThreshUpperInt, 100);
  createTrackbar("vThreshLower", "Chromacity", &vThreshLowerInt, 100);

  // Physical
  int coneAngleInt = 157;             // /1000 (0.15707963267)
  int coneR1Int = 300;                // /1000 (0.3)
  int coneR2Int = 1000;               // /1000 (1.0)
  int gmmLearningRateInt = 100;       // /1000 (0.1)
  int gmmAccumWeightThreshInt = 700;  // /1000 (0.7)
  int postThreshInt = 150;            // /1000 (.15)

  int weightSmootTermInt = 4;
  int gmmInitVarInt = 30;
  int gmmStdThresholdInt = 2;

  createTrackbar("coneAngle", "Physical", &coneAngleInt, round(CV_PI*1000));
  createTrackbar("coneR1", "Physical", &coneR1Int, 1000);
  createTrackbar("coneR2", "Physical", &coneR2Int, 2000);
  createTrackbar("weightSmootTerm", "Physical", &weightSmootTermInt, 8); // (=4)
  createTrackbar("gmmGaussians", "Physical", &phys.params.gmmGaussians, 10); // (=5)
  createTrackbar("gmmInitVar", "Physical", &gmmInitVarInt, 60); // (=30)
  createTrackbar("gmmStdThreshold", "Physical", &gmmStdThresholdInt, 10); // (=2)
  createTrackbar("gmmLearningRate", "Physical", &gmmLearningRateInt, 1000);
  createTrackbar("gmmAccumWeightThresh", "Physical", &gmmAccumWeightThreshInt, 1000);
  createTrackbar("postThresh", "Physical", &postThreshInt, 1000);

  // Geometry
  int gWeightInt = 70;
  int sRelativeWeightInt = 20;
  int thresholdScaleInt = 40;

  createTrackbar("smoothFactor", "Geometry", &geo.params.smoothFactor, 20);
  createTrackbar("headThreshRatio", "Geometry", &geo.params.headThreshRatio, 20);
  createTrackbar("minHeadSeq", "Geometry", &geo.params.minHeadSeq, 20);
  createTrackbar("maxEdgeDistance", "Geometry", &geo.params.maxEdgeDistance, 20);
  createTrackbar("edgeThreshRatio", "Geometry", &geo.params.edgeThreshRatio, 20);
  createTrackbar("minEdgeSeq", "Geometry", &geo.params.minEdgeSeq, 20);
  createTrackbar("bottomShiftRatio", "Geometry", &geo.params.bottomShiftRatio, 20);
  createTrackbar("gWeight", "Geometry", &gWeightInt, 100);
  createTrackbar("sRelativeWeightInt", "Geometry", &sRelativeWeightInt, 100);
  createTrackbar("thresholdScaleInt", "Geometry", &thresholdScaleInt, 100);

  // Large Region
  int avgSatThreshInt = 35;
  int avgAttenThreshInt = 158;
  int vThreshUpperLowAttenInt = 100;
  int vThreshUpperHighAttenInt = 99;
  int vThreshLowerLowAttenInt = 60;
  int vThreshLowerHighAttenInt = 121;

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

  createTrackbar("vThreshUpperLowAtten", "Large Region Texture", &vThreshUpperLowAttenInt, 200);
  createTrackbar("vThreshUpperHighAtten", "Large Region Texture", &vThreshUpperHighAttenInt, 200);
  createTrackbar("vThreshLowerLowAtten", "Large Region Texture", &vThreshLowerLowAttenInt, 200);
  createTrackbar("vThreshLowerHighAtten", "Large Region Texture", &vThreshLowerHighAttenInt, 200);
  createTrackbar("avgAttenThresh", "Large Region Texture", &avgAttenThreshInt, 500);

  //createTrackbar("avgPerimThresh", "Large Region Texture", &avgPerimThreshInt, 500);
  //createTrackbar("edgeDiffRadius", "Large Region Texture", &lrTex.params.edgeDiffRadius, 10);
  //createTrackbar("borderDiffRadius", "Large Region Texture", &lrTex.params.borderDiffRadius, 10);
  //createTrackbar("splitIncrement", "Large Region Texture", &lrTex.params.splitIncrement, 10);
  //createTrackbar("splitRadius", "Large Region Texture", &lrTex.params.splitRadius, 10);

  //createTrackbar("cannyThresh1", "Large Region Texture", &cannyThresh1Int, 200);
  //createTrackbar("cannyThresh2", "Large Region Texture", &cannyThresh2Int, 200);
  //createTrackbar("cannyApertureSize", "Large Region Texture", &lrTex.params.cannyApertureSize, 10);

  //createTrackbar("minCorrPoints", "Large Region Texture", &lrTex.params.minCorrPoints, 10);
  //createTrackbar("maxCorrRounds", "Large Region Texture", &lrTex.params.maxCorrRounds, 10);
  //createTrackbar("corrBorder", "Large Region Texture", &lrTex.params.corrBorder, 10);

  //createTrackbar("gradScales", "Large Region Texture", &lrTex.params.gradScales, 10);
  //createTrackbar("gradMagThresh", "Large Region Texture", &gradMagThreshInt, 10);
  //createTrackbar("gradAttenThresh", "Large Region Texture", &gradAttenThreshInt, 200);
  //createTrackbar("gradDistThresh", "Large Region Texture", &gradDistThreshInt, 200);
  //createTrackbar("gradCorrThreshLowAtten", "Large Region Texture", &gradCorrThreshLowAttenInt, 100);
  //createTrackbar("gradCorrThreshHighAtten", "Large Region Texture", &gradCorrThreshHighAttenInt, 100);

  // processing loop
  for(;;) {

    /* PRE-PROCESSING */

    // UPDATE FLOAT PARAMS
    chr.params.vThreshUpper = (float)vThreshUpperInt / 100.0;
    chr.params.vThreshLower = (float)vThreshLowerInt / 100.0;

    phys.params.coneAngle = (float)coneAngleInt / 1000.0;
    phys.params.coneR1 = (float)coneR1Int / 1000.0;
    phys.params.coneR2 = (float)coneR2Int / 1000.0;
    phys.params.weightSmootTerm = (float)weightSmootTermInt;
    phys.params.gmmInitVar = (float)gmmInitVarInt;
    phys.params.gmmLearningRate = (float)gmmLearningRateInt / 1000.0;
    phys.params.gmmAccumWeightThresh = (float)gmmAccumWeightThreshInt / 1000.0;
    phys.params.postThresh = (float)postThreshInt / 1000.0;
    phys.params.gmmStdThreshold = (float)gmmStdThresholdInt;

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

    geo.params.gWeight = (float)gWeightInt/100;
    geo.params.sRelativeWeight = (float)sRelativeWeightInt/100;
    geo.params.thresholdScale = (float)thresholdScaleInt/100;

    /* PROCESSING */

#ifdef PRECALC
    Mat shadowMask = Mat::zeros(frame.rows, frame.cols, CV_8U);

	  chr.removeShadows(frame, fg, bg, chrMask, shadowMask);
	  lrTex.removeShadows(frame, fg, bg, lrTexMask, shadowMask);
#else
	  chr.removeShadows(frame, fg, bg, chrMask);
    phys.removeShadows(frame, fg, bg, physMask);
	  lrTex.removeShadows(frame, fg, bg, lrTexMask);
    geo.removeShadows(frame, fg, bg, geoMask);
#endif

    /* POST-PROCESSING */

    stringstream str;

    str << "Detection Rate: " << 100*calcDetectRate(physMask, shadows);
    putText(physMask, str.str(), Point(10,25), FONT_HERSHEY_COMPLEX_SMALL, 0.8,
            Scalar(50,50,200), 1, CV_AA);

    str.str("");
    str << "Discrimination Rate: " << 100*calcDiscrimRate(physMask, shadows);
    putText(physMask, str.str(), Point(10,45), FONT_HERSHEY_COMPLEX_SMALL, 0.8,
            Scalar(50,50,200), 1, CV_AA);

    str.str("");
    str << "Score: " << 100*calcDiscrimRate(physMask, shadows) + 100*calcDetectRate(physMask, shadows);
    putText(physMask, str.str(), Point(10,65), FONT_HERSHEY_COMPLEX_SMALL, 0.8,
            Scalar(50,50,200), 1, CV_AA);

	  // show results
	  //imshow("Frame", frame);
    //imshow("Ground Truth", shadows);
	  //imshow("Chromacity", chrMask);
	  imshow("Physical", physMask);
    //imshow("Geometry", geoMask);
	  //imshow("Large Region Texture", lrTexMask);


    if(waitKey(30) == 'q') break;
  }
	return 0;
}
