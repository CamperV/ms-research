// ---
// Jay Danner
//
// This program runs evnparameters on foreground objects only.
// NOTE: run with the ../shadowdetection/samples instead!
// ---

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {

  if(argc > 3) {
#ifdef DEBUG
    cout << "\n'Oak's words echoed... there is a time and a place for everything!' Run with $shadowdetection samples!" << endl;
    cout << "\n---" << endl;
    cout << "> Opening image " << argv[1] << "..." << endl;
    cout << "> Opening background " << argv[2] << "..." << endl;
    cout << "> Opening shadows " << argv[3] << "..." << endl;
#endif
  } else {
    cout << "Not enough inputs specified: exiting" << endl;
    exit(0);
  }

  Mat img = imread(argv[1], CV_LOAD_IMAGE_COLOR);
  Mat bg = imread(argv[2]);
  Mat shadows = imread(argv[3], CV_LOAD_IMAGE_GRAYSCALE);

  if (!img.data) { 
    cout << "Image failed to open." << endl; 
    return -1; 
  }

  // thresh shadows to fg objects
  threshold(shadows, shadows, 0, 255, THRESH_BINARY);

  Mat fg_img;
  img.copyTo(fg_img, shadows);

  Mat imgHSV;
  cvtColor(fg_img, imgHSV, CV_RGB2HSV);

  /* PROCESSING */

  vector<Mat> channels(3);
  split(imgHSV, channels);

  float hmean, smean, vmean;
  int pixcnt;
  float val;

  for(int i = 0; i < 3; i++) {
    pixcnt = 0;
    val = 0;
    for(int r = 0; r < channels[i].rows; r++) {
    for(int c = 0; c < channels[i].cols; c++) {

      if((float)channels[i].at<unsigned char>(r, c) != 0) {
        val += (float)channels[i].at<unsigned char>(r, c);
        pixcnt++;
      }

    }
    }

    switch(i) {
      case 0:
        hmean = val/pixcnt;
        break;
      case 1:
        smean = val/pixcnt;
        break;
      case 2:
        vmean = val/pixcnt;
        break;
    }
  }
#ifdef DEBUG
  cout << "Avg Hue: " << hmean << "\t\tAvg Sat: " << smean << "\t\tAvg Val: " << vmean << endl;
#endif
#ifndef DEBUG
  cout << hmean << "," << smean << "," << vmean << endl;
#endif

	return 0;
}
