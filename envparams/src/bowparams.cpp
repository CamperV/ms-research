// ---
// Jay Danner
// ---

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/features2d/features2d.hpp"

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
  SiftFeatureDetector detector;

  // get keypoints
  vector<KeyPoint> keypoints;
  detector.detect(img, keypoints);

  // get descriptors
  Mat descriptors;
  detector.compute(img, keypoints, descriptors);
  
  // start BoW
  int numKlusters = 1000;
  BOWKMeansTrainer bowtrainer(numKlusters);
  bowtrainer.add(descriptors);
  Mat vocab = bowtrainer.cluster();

  // find response histogram
  Mat response_histogram;
  // --- needed for constructor --- //
  Ptr<DescriptorMatcher> matcher(new BFMatcher(NORM_L2));
  Ptr<DescriptorExtractor > extractor(Ptr<DescriptorExtractor>(new SiftDescriptorExtractor()));
  // --- needed for constructor --- //
  BOWImgDescriptorExtractor bowide(extractor, matcher);
  bowide.setVocabulary(vocab);
  bowide.compute(img, keypoints, response_histogram);

  // operate on histogram
  Scalar mean, stddev;
  meanStdDev(response_histogram, mean, stddev);
    
  cout << "dr: " << descriptors.rows << ", dc: " << descriptors.cols << endl;
  cout << "vr: " << vocab.rows << ", vc: " << vocab.cols << endl;

  cout << "Mean: " << mean[0] << endl;
  cout << "Std Dev: " << stddev[0] << endl;

#ifdef DEBUG
  cout << "Number of : " << endl;
#endif
#ifndef DEBUG
  cout << endl;
#endif
	return 0;
}
