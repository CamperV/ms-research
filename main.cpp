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
	PhysicalShadRem phy;
	GeometryShadRem geo;
	SrTextureShadRem srTex;
	LrTextureShadRem lrTex;

	// matrices to store the masks after shadow removal
	Mat chrMask, phyMask, geoMask, srTexMask, lrTexMask;

  BackgroundSubtractorMOG2 MOG = BackgroundSubtractorMOG2();
  MOG.set("detectShadows", false);
  MOG.set("nmixtures", 3);

  bool step = false;
  // processing loop
  for(;;) {

    /* PRE-PROCESSING */

    capture >> frame;

    // check if we need to restart the video
    if(frame.empty()) {
        // Looks like we've hit the end of our feed! Restart
        cout << "Frame is empty, restarting..." << endl;
        capture.set(CV_CAP_PROP_POS_AVI_RATIO, 0.0);
        cout << "Done!" << endl;
        continue;
    }

    MOG(frame, fg);
    MOG.getBackgroundImage(bg);

    /* PROCESSING */

	  chr.removeShadows(frame, fg, bg, chrMask);
	  //phy.removeShadows(frame, fg, bg, phyMask);
	  //geo.removeShadows(frame, fg, bg, geoMask);
	  //srTex.removeShadows(frame, fg, bg, srTexMask);
	  lrTex.removeShadows(frame, fg, bg, lrTexMask);

    /* POST-PROCESSING */

    vstats.updateFPS();
    vstats.displayStats();

	  // show results
	  imshow("frame", frame);
	  imshow("fg", fg);
	  imshow("chrMask", chrMask);
	  imshow("lrTexMask", lrTexMask);

    if(step) {
      if(waitKey(0) == 115) step = !step;
    } else {
      if(waitKey(30) == 115) step = !step;
    }
  }
	return 0;
}
