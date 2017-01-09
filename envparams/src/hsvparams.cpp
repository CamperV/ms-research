// ---
// Jay Danner
// ---

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
  
  string infile = "";
  Mat img, out;

  if(argc > 1) {
    infile = argv[1];
  } else {
    cout << "No input file specified: using DEFAULT." << endl;
    infile = "";
    exit(0);
  }

  img = imread(infile, CV_LOAD_IMAGE_COLOR);

#ifdef DEBUG
  cout << "Opening " << infile << "..." << endl;
#endif

  if (!img.data) { 
    cout << "Image failed to open." << endl; 
    return -1; 
  }

  // init windows
  //namedWindow("Image 1", 1);

  Mat imgHSV;
  cvtColor(img, imgHSV, CV_RGB2HSV);

  /* PROCESSING */

  /*
   * Possible environmental parameters:
   * (i) Texture density
   * (ii) Illumination
   * (iii) average chromacity
   * (iv) contrast
   **/

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

  /*
  for(;;) {
	  // show results
	  imshow("Image 1", out);
    if(waitKey(0) == 27) break;
  }
  */
	return 0;
}
