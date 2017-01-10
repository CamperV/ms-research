// ---
// Jay Danner
// Takes in an mp4 stream and outputs foreground frames.
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

  if (!frame.data) { 
    cout << "Capture failed to open." << endl; 
    return -1; 
  }

  Mat fg, bg;
  BackgroundSubtractorMOG2 MOG = BackgroundSubtractorMOG2();
  MOG.set("detectShadows", 0);
  MOG.set("nmixtures", 5);

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
   
    MOG(frame, fg, 0.005);
    MOG.getBackgroundImage(bg);

    // write stream name
    stringstream ss;
    ss << "output/PETs1/fg/" << setw(5) << setfill('0') << framecounter << ".jpg";
    cout << ss.str() << endl;

    erode(fg, fg, Mat(), Point(-1,-1), 1); 
    dilate(fg, fg, Mat(), Point(-1,-1), 1); 

    // write out frames
    imwrite(ss.str(), fg);

    framecounter++;
  }
	return 0;
}
