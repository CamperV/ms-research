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
#include <iomanip>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/video/background_segm.hpp>

#include "ChromacityShadRem.h"
#include "GeometryShadRem.h"
#include "LrTextureShadRem.h"
#include "PhysicalShadRem.h"
#include "SrTextureShadRem.h"

#include "Metrics.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
  
  string infile = "";
  if(argc > 1) {
    cout << "\n---" << endl;
    cout << "> Opening stream " << argv[1] << "..." << endl;
  } else {
    cout << "Not enough inputs specified: exiting" << endl;
    exit(0);
  }

  VideoCapture capture(argv[1]);

  Mat frame;
  capture >> frame;

  namedWindow("Foreground", 1);

  if (!frame.data) { 
    cout << "Capture failed to open." << endl; 
    return -1; 
  }

  Mat fg, bg;
  BackgroundSubtractorMOG2 MOG = BackgroundSubtractorMOG2();
  MOG.set("detectShadows", 0);
  MOG.set("nmixtures", 5);

  int ft = 0;
  createTrackbar("ftau", "Foreground", &ft, 100);

  double _ft;

	// create shadow removers
	ChromacityShadRem chr;
  PhysicalShadRem phys;
  GeometryShadRem geo;
	SrTextureShadRem srTex;
	LrTextureShadRem lrTex;

	// matrices to store the masks after shadow removal
	Mat chrMask, physMask, geoMask, srTexMask, lrTexMask;

  // processing loop
  for(;;) {

    capture >> frame;

    _ft = ft/100.0;
    MOG.set("fTau", 0.65);

    MOG(frame, fg);
    MOG.getBackgroundImage(bg);

    erode(fg, fg, Mat(), Point(-1,-1)); 
    dilate(fg, fg, Mat(), Point(-1,-1), 2); 
    erode(fg, fg, Mat(), Point(-1,-1)); 
    dilate(fg, fg, Mat(), Point(-1,-1), 1); 


    // remove shadows
	  chr.removeShadows(frame, fg, bg, chrMask);
	  phys.removeShadows(frame, fg, bg, physMask);
	  geo.removeShadows(frame, fg, bg, geoMask);
    //srTex.removeShadows(frame, fg, bg, srTexMask);
    lrTex.removeShadows(frame, fg, bg, lrTexMask);

    /*--------*/
    /* OUTPUT */
    /*--------*/
    /*
    cout << "\nDetection and Discrimination Rates:" << endl;
    cout << "-----------------------------------" << endl;
    cout << setprecision(4) << fixed 
         << "Chromacity:\t("    << 100*calcDetectRate(chrMask, shadows)     << ",\t" 
                                << 100*calcDiscrimRate(chrMask, shadows)    << ")"  << endl;
    cout << "Physical:\t("      << 100*calcDetectRate(physMask, shadows)    << ",\t" 
                                << 100*calcDiscrimRate(physMask, shadows)   << ")"  << endl;
    cout << "Geometry:\t("      << 100*calcDetectRate(geoMask, shadows)     << ",\t" 
                                << 100*calcDiscrimRate(geoMask, shadows)    << ")"  << endl;
    cout << "Small Region:\t("  << 100*calcDetectRate(srTexMask, shadows)   << ",\t" 
                                << 100*calcDiscrimRate(srTexMask, shadows)  << ")"  << endl;
    cout << "Large Region:\t("  << 100*calcDetectRate(lrTexMask, shadows)   << ",\t" 
                                << 100*calcDiscrimRate(lrTexMask, shadows)  << ")"  << endl;
    */

    /*--------*/
    /* STDERR */
    /*--------*/
    /*
    cerr << setprecision(4) << fixed
         << "C,"                << 100*calcDetectRate(chrMask, shadows)     << "," 
                                << 100*calcDiscrimRate(chrMask, shadows)    << endl;
    cerr << "P,"                << 100*calcDetectRate(physMask, shadows)    << "," 
                                << 100*calcDiscrimRate(physMask, shadows)   << endl;
    cerr << "G,"                << 100*calcDetectRate(geoMask, shadows)     << "," 
                                << 100*calcDiscrimRate(geoMask, shadows)    << endl;
    cerr << "S,"               << 100*calcDetectRate(srTexMask, shadows)   << "," 
                                << 100*calcDiscrimRate(srTexMask, shadows)  << endl;
    cerr << "L,"               << 100*calcDetectRate(lrTexMask, shadows)   << "," 
                                << 100*calcDiscrimRate(lrTexMask, shadows)  << endl;
    */

	  // show results
	  imshow("Frame", frame);
    imshow("Foreground", fg);
    //imshow("Background", bg);
	  //imshow("Chromacity", chrMask);
	  //imshow("Physical", physMask);
	  //imshow("Geometry", geoMask);
	  //imshow("Small Region Texture", srTexMask);
	  //imshow("Large Region Texture", lrTexMask);

    if(waitKey(30) == 'q') break;
  }
	return 0;
}
