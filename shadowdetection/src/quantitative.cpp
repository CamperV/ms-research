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
  if(argc > 3) {
    cout << "\n---" << endl;
    cout << "> Opening image " << argv[1] << "..." << endl;
    cout << "> Opening background " << argv[2] << "..." << endl;
    cout << "> Opening shadows " << argv[3] << "..." << endl;
  } else {
    cout << "Not enough inputs specified: exiting" << endl;
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

  /** USED FOR ABSDIFF FG **
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

  // remove shadows
	chr.removeShadows(frame, fg, bg, chrMask);
	phys.removeShadows(frame, fg, bg, physMask);
	geo.removeShadows(frame, fg, bg, geoMask);
  srTex.removeShadows(frame, fg, bg, srTexMask);
  lrTex.removeShadows(frame, fg, bg, lrTexMask);

  /*--------*/
  /* OUTPUT */
  /*--------*/

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

  /*--------*/
  /* STDERR */
  /*--------*/

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

  // processing loop
  /*
  for(;;) {
	  // show results
	  imshow("Frame", frame);
	  imshow("Ground Truth", shadows);
    imshow("Foreground", fg);
    imshow("Background", bg);
	  imshow("Chromacity", chrMask);
	  imshow("Physical", physMask);
	  imshow("Geometry", geoMask);
	  imshow("Small Region Texture", srTexMask);
	  imshow("Large Region Texture", lrTexMask);

    if(waitKey(30) == 'q') break;
  }
  */
	return 0;
}
