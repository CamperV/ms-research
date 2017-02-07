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
vector<KeyPoint> vectorSIFT(Mat img);

int main(int argc, char** argv) {

  string infile = "";
  string infile2 = "";
  bool view = false;
  Mat img, bg, out;

  if(argc > 1) {
    infile = argv[1];
    infile2 = argv[2];
    view = (bool)atoi(argv[3]);
  } else {
    cout << "No error message yet, because Jay is busy. Stop asking!" << endl;
    exit(0);
  }

  img = imread(infile, CV_LOAD_IMAGE_GRAYSCALE);
  bg = imread(infile2, CV_LOAD_IMAGE_GRAYSCALE);

#ifdef DEBUG
  cout << "Opening " << infile << "..." << endl;
  cout << "Opening " << bg << "..." << endl;
#endif

  if (!img.data || !bg.data) {
    cout << "Image failed to open." << endl;
    return -1;
  }

  //float ratio = ratioSIFT(img, 0.01);
  //float bg_ratio = ratioSIFT(bg, 0.01);
  float ratio = ratioSIFT(img, 0.04);
  float bg_ratio = ratioSIFT(bg, 0.04);

  cout  << ratio << ","
        << bg_ratio << ","
        << ratio-bg_ratio
        << endl;

  if(view) {
    vector<KeyPoint> remainingSIFT = vectorSIFT(img);
    vector<KeyPoint> remainingSIFT_bg = vectorSIFT(bg);
    Mat kpImg, kpImg_bg;

    // draw
    drawKeypoints(img, remainingSIFT, kpImg, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    drawKeypoints(bg, remainingSIFT_bg, kpImg_bg, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    cout << "Displaying!" << endl;
    cout << (int)remainingSIFT.size() - (int)remainingSIFT_bg.size() << endl;
    while(1) {
      imshow("kpImg", kpImg);
      imshow("kpImg_bg", kpImg_bg);
      waitKey(30);
    }
  }

	return 0;
}

vector<KeyPoint> vectorSIFT(Mat img) {
  // start SIFT analysis
  int numSIFT;
  int numSIFT_lowc;

  // default is 0.04 for contrast threshold
  SiftFeatureDetector detector;
  //SiftFeatureDetector detector_lowc(0,3,0.01,10,1.6);
  SiftFeatureDetector detector_lowc(0,3,0.04,50,1.6);

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

  // Create Mat representing 'high' contrast SIFT keypoints
  // Iterate through LowC keypoints and remove all that occupy the same coordinates
  // Display the remaining points with DRAW_RICH_KEYPOINTS
  // Should give 1) location 2) orientation 3) size
  Mat keyMat = Mat(img.rows, img.cols, CV_8U, Scalar(0,0,0));
  for(int i=0; i<keypoints.size(); i++) {
    Point2f p = keypoints[i].pt;
    keyMat.at<int>(p) = 255;
  }

  vector<KeyPoint> remainingSIFT;
  for(int j=0; j<keypoints_lowc.size(); j++) {
    Point2f p = keypoints_lowc[j].pt;
    if(keyMat.at<int>(p) == 255) {
      continue;
    }
    remainingSIFT.push_back(keypoints_lowc[j]);
  }

  return remainingSIFT;
  //return keypoints;
  //return keypoints_lowc;
}

float ratioSIFT(Mat img, float contrastRatio) {

  // start SIFT analysis
  int numSIFT;
  int numSIFT_lowc;

  // default is 0.04 for contrast threshold
  SiftFeatureDetector detector;
  //SiftFeatureDetector detector_lowc(0,3,contrastRatio,10,1.6);
  SiftFeatureDetector detector_lowc(0,3,contrastRatio,50,1.6);

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

  return numSIFT - numSIFT_lowc;
  //return 1-((float)numSIFT/(float)numSIFT_lowc);
}
