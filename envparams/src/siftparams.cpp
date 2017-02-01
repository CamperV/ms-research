// ---
// Jay Danner
// ---

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"

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

  img = imread(infile, CV_LOAD_IMAGE_GRAYSCALE);

#ifdef DEBUG
  cout << "Opening " << infile << "..." << endl;
#endif

  if (!img.data) { 
    cout << "Image failed to open." << endl; 
    return -1; 
  }

  // start SIFT analysis
  int numSIFT;
  int numSIFT_lowc;
  SiftFeatureDetector detector;
  SiftFeatureDetector detector_lowc(0,3,0.01,10,1.6);

  vector<KeyPoint> keypoints;
  detector.detect(img, keypoints);

  vector<KeyPoint> keypoints_lowc;
  detector_lowc.detect(img, keypoints_lowc);

  Mat descriptors;
  detector.compute(img, keypoints, descriptors);

  numSIFT = keypoints.size();

  Mat descriptors_lowc;
  detector_lowc.compute(img, keypoints_lowc, descriptors_lowc);

  numSIFT_lowc = keypoints_lowc.size();

#ifdef DEBUG
  cout << "Number of SIFT features: " << numSIFT << endl;
#endif
#ifndef DEBUG
  cout << numSIFT << "," << numSIFT_lowc << "," 
       << numSIFT_lowc-numSIFT << "," << (float)numSIFT/(float)numSIFT_lowc << endl;
#endif
	return 0;
}
