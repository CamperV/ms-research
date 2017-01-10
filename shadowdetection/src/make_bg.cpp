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
#include <unistd.h>
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

  int lr = 0;
  createTrackbar("Learning Rate", "Foreground", &lr, 100);

  double _lr;

	// create shadow removers
	ChromacityShadRem chr;
  PhysicalShadRem phys;
  GeometryShadRem geo;
	SrTextureShadRem srTex;
	LrTextureShadRem lrTex;

	// matrices to store the masks after shadow removal
	Mat chrMask, physMask, geoMask, srTexMask, lrTexMask;

  int framecounter = 0;
  // processing loop
  for(;;) {

    capture >> frame;

    GaussianBlur(frame, frame, Size(5,5), 0, 0);

    _lr = lr/100.0;

   
    //MOG(frame, fg);
    //MOG(frame, fg, _lr);
    MOG(frame, fg, 0.005);
    MOG.getBackgroundImage(bg);

    // write stream name
    stringstream ss;
    ss << "bg/" << setw(5) << setfill('0') << framecounter << ".jpg";
    cout << ss.str() << endl;

    erode(fg, fg, Mat(), Point(-1,-1), 1); 
    dilate(fg, fg, Mat(), Point(-1,-1), 1); 

	  // show results
	  imshow("Frame", frame);
    imshow("Foreground", fg);
    imshow("Background", bg);
    
    imwrite(ss.str(), bg);

    framecounter++;
    if(waitKey(1000) == 'q') break;
  }
	return 0;
}
