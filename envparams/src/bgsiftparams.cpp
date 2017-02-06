// ---
// Jay Danner
// ---

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"

using namespace std;
using namespace cv;

float ratioSIFT(Mat img, float contrastRatio);

int main(int argc, char** argv) {

  string infile = "";
  string infile2 = "";
  Mat img, bg, out;

  if(argc > 1) {
    infile = argv[1];
    infile2 = argv[2];
  } else {
    cout << "No input file specified: using DEFAULT." << endl;
    infile = "";
    exit(0);
  }

  img = imread(infile, CV_LOAD_IMAGE_GRAYSCALE);
  bg = imread(infile2, CV_LOAD_IMAGE_GRAYSCALE);

#ifdef DEBUG
  cout << "Opening " << infile << "..." << endl;
  cout << "Opening " << bg << "..." << endl;
#endif

  //if (!img.data || !bg.data) {
  if (!img.data) {
    cout << "Image failed to open." << endl;
    return -1;
  }

float ratio = ratioSIFT(img, 0.01);
float bg_ratio = ratioSIFT(bg, 0.01);

cout    << ratio << ","
        << bg_ratio << ","
        << ratio-bg_ratio
        << endl;
	return 0;
}

float ratioSIFT(Mat img, float contrastRatio) {

  // start SIFT analysis
  int numSIFT;
  int numSIFT_lowc;

  // default is 0.04 for contrast threshold
  SiftFeatureDetector detector;
  SiftFeatureDetector detector_lowc(0,3,contrastRatio,10,1.6);

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
  detector_lowc.detect(img, keypoints_lowc);
  detector_lowc.compute(img, keypoints_lowc, descriptors_lowc);
  ///
  numSIFT_lowc = keypoints_lowc.size();

  return 1-((float)numSIFT/(float)numSIFT_lowc);
}
