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
  int numSIFT_lowc0, numSIFT_lowc1, numSIFT_lowc2, numSIFT_lowc3;

  // default is 0.04 for contrast threshold
  SiftFeatureDetector detector;     
  SiftFeatureDetector detector_lowc0(0,3,0.00,10,1.6);
  SiftFeatureDetector detector_lowc1(0,3,0.01,10,1.6);
  SiftFeatureDetector detector_lowc2(0,3,0.02,10,1.6);
  SiftFeatureDetector detector_lowc3(0,3,0.03,10,1.6);

  vector<KeyPoint> keypoints;
  Mat descriptors;
  ///
  detector.detect(img, keypoints);
  detector.compute(img, keypoints, descriptors);
  ///
  numSIFT = keypoints.size();

  vector<KeyPoint> keypoints_lowc;
  Mat descriptors_lowc;
  ///
  detector_lowc0.detect(img, keypoints_lowc);
  detector_lowc0.compute(img, keypoints_lowc, descriptors_lowc);
  ///
  numSIFT_lowc0 = keypoints_lowc.size();

  ///
  detector_lowc1.detect(img, keypoints_lowc);
  detector_lowc1.compute(img, keypoints_lowc, descriptors_lowc);
  ///
  numSIFT_lowc1 = keypoints_lowc.size();

  ///
  detector_lowc2.detect(img, keypoints_lowc);
  detector_lowc2.compute(img, keypoints_lowc, descriptors_lowc);
  ///
  numSIFT_lowc2 = keypoints_lowc.size();

  ///
  detector_lowc3.detect(img, keypoints_lowc);
  detector_lowc3.compute(img, keypoints_lowc, descriptors_lowc);
  ///
  numSIFT_lowc3 = keypoints_lowc.size();

#ifdef DEBUG
  cout << "Number of SIFT features: " << numSIFT << endl;
#endif
#ifndef DEBUG
  cout << numSIFT << ","
       << 1-((float)numSIFT/(float)numSIFT_lowc0) << "," 
       << 1-((float)numSIFT/(float)numSIFT_lowc1) << "," 
       << 1-((float)numSIFT/(float)numSIFT_lowc2) << "," 
       << 1-((float)numSIFT/(float)numSIFT_lowc3)
       << endl;
#endif
	return 0;
}
