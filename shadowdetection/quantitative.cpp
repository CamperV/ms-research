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
#include "opencv2/imgproc/imgproc.hpp"

#include "ChromacityShadRem.h"
#include "GeometryShadRem.h"
#include "LrTextureShadRem.h"
#include "PhysicalShadRem.h"
#include "SrTextureShadRem.h"

using namespace std;
using namespace cv;

double calcDetectRate(Mat mask, Mat shadows) {
  // all concerning shadows
  // TP/TN - True Positive/Negative
  // FP/FN - False Positive/Negative

  int TPs = -1;
  int FNs = -1;

  assert(mask.rows == shadows.rows);
  assert(mask.cols == shadows.cols);

  for(int r = 0; r < mask.rows; r++) {
    for(int c = 0; c < mask.cols; c++) {
      int maskval = mask.at<unsigned char>(r, c);
      int shadval = shadows.at<unsigned char>(r, c);

      if(maskval == (char)0 && shadval == (char)127) FNs++;
      if(maskval == (char)255 && shadval == (char)127) FNs++;
      if(maskval == (char)127 && shadval == (char)127) TPs++;
    }
  }
  return TPs/(FNs+TPs);
}

double calcDiscrimRate(Mat mask, Mat shadows) {
  // all concerning foreground
  // TP/TN - True Positive/Negative
  // FP/FN - False Positive/Negative

  int TPf = -1;
  int FNf = -1;

  assert(mask.rows == shadows.rows);
  assert(mask.cols == shadows.cols);

  for(int r = 0; r < mask.rows; r++) {
    for(int c = 0; c < mask.cols; c++) {
      char maskval = mask.at<unsigned char>(r, c);
      char shadval = shadows.at<unsigned char>(r, c);

      if(maskval == (char)0 && shadval == (char)255) FNf++;
      if(maskval == (char)127 && shadval == (char)255) FNf++;
      if(maskval == (char)255 && shadval == (char)255) TPf++;
    }
  }
  return TPf/(FNf+TPf);
}

int main(int argc, char** argv) {
  
  string infile = "";
  if(argc > 3) {
    cout << "Opening image " << argv[1] << "..." << endl;
    cout << "Opening background " << argv[2] << "..." << endl;
    cout << "Opening shadows " << argv[3] << "..." << endl;
  } else {
    cout << "Not enough inputs specified: exiting" << endl;
    exit(0);
  }

  Mat frame = imread(argv[1]);
  Mat bg = imread(argv[2]);
  Mat shadows = imread(argv[3]);

  if (!frame.data) { 
    cout << "Capture failed to open." << endl; 
    return -1; 
  }

  Mat fg;
  absdiff(frame, bg, fg);
  cvtColor(fg, fg, CV_BGR2GRAY);
  threshold(fg, fg, 25, 255, THRESH_BINARY);

  erode(fg, fg, Mat(), Point(-1,-1)); 
  dilate(fg, fg, Mat(), Point(-1,-1)); 

	// create shadow removers
	ChromacityShadRem chr;
  PhysicalShadRem phys;
  GeometryShadRem geo;
	SrTextureShadRem srTex;
	LrTextureShadRem lrTex;

	// matrices to store the masks after shadow removal
	Mat chrMask, physMask, geoMask, srTexMask, lrTexMask;

	chr.removeShadows(frame, fg, bg, chrMask);
	phys.removeShadows(frame, fg, bg, physMask);
	geo.removeShadows(frame, fg, bg, geoMask);
  srTex.removeShadows(frame, fg, bg, srTexMask);
  lrTex.removeShadows(frame, fg, bg, lrTexMask);

  cout << "\nDetection and Discrimination Rates:" << endl;
  cout << "-----------------------------------" << endl;
  cout << "Chromacity:\t(" << calcDetectRate(chrMask, shadows) << ", " << calcDiscrimRate(chrMask, shadows) << ")" << endl;
  cout << "Physical:\t(" << calcDetectRate(physMask, shadows) << ", " << calcDiscrimRate(physMask, shadows) << ")" << endl;
  cout << "Geometry:\t(" << calcDetectRate(geoMask, shadows) << ", " << calcDiscrimRate(geoMask, shadows) << ")" << endl;
  cout << "Small Region:\t(" << calcDetectRate(srTexMask, shadows) << ", " << calcDiscrimRate(srTexMask, shadows) << ")" << endl;
  cout << "Large Region:\t(" << calcDetectRate(lrTexMask, shadows) << ", " << calcDiscrimRate(lrTexMask, shadows) << ")" << endl;

  // processing loop
  for(;;) {
	  // show results
	  imshow("Frame", frame);
	  imshow("Ground Truth", shadows);
	  imshow("Chromacity", chrMask);
	  imshow("Physical", physMask);
	  imshow("Geometry", geoMask);
	  imshow("Small Region Texture", srTexMask);
	  imshow("Large Region Texture", lrTexMask);

    if(waitKey(30) == 'q') break;
  }
	return 0;
}
