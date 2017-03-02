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
void showHSV(Mat in, vector<Mat>& channels);
void showChannels(Mat in, vector<Mat>& channel, std::string labels[], double scalar[][3], int convertBack);

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

  img = imread(infile, CV_LOAD_IMAGE_COLOR);
  bg = imread(infile2, CV_LOAD_IMAGE_COLOR);

#ifdef DEBUG
  cout << "Opening " << infile << "..." << endl;
  cout << "Opening " << bg << "..." << endl;
#endif

  if (!img.data || !bg.data) {
    cout << "Image failed to open." << endl;
    return -1;
  }

  // convert to HSV
  Mat hsv_img;
  vector<Mat> hsv_channels;
  cvtColor(img, hsv_img, CV_BGR2HSV);
  split(hsv_img, hsv_channels);

  Mat hsv_bg;
  vector<Mat> hsv_bg_channels;
  cvtColor(bg, hsv_bg, CV_BGR2HSV);
  split(hsv_bg, hsv_bg_channels);

  // display
  //showHSV(hsv_img, hsv_channels);

  float ratio = ratioSIFT(img, 0.01);
  float bg_ratio = ratioSIFT(bg, 0.01);

  float rH = ratioSIFT(hsv_channels[0], 0.01);
  float rS = ratioSIFT(hsv_channels[1], 0.01);
  float rV = ratioSIFT(hsv_channels[2], 0.01);

  float bgrH = ratioSIFT(hsv_bg_channels[0], 0.01);
  float bgrS = ratioSIFT(hsv_bg_channels[1], 0.01);
  float bgrV = ratioSIFT(hsv_bg_channels[2], 0.01);

  //cout  << ratio << ","
  //      << bg_ratio << ","
  //      << ratio-bg_ratio << ","
  //      << rH << ","
  cout << rS << ","
  //      << rV << ","
  //      << bgrH << ","
        << bgrS << ","
        << (float)rS/(float)bgrS
  //      << bgrV
        << endl;

  // for aArS
  //cout << (float)rS/(float)bgrS << endl;

  // only for visualization
  if(view) {
    Mat kpImg, kpImg_bg;
    Mat kpH, kpS, kpV;
    Mat kpbgH, kpbgS, kpbgV;

    vector<KeyPoint> remainingSIFT = vectorSIFT(img);
    vector<KeyPoint> remainingSIFT_bg = vectorSIFT(bg);

    vector<KeyPoint> remainingSIFT_H = vectorSIFT(hsv_channels[0]);
    vector<KeyPoint> remainingSIFT_S = vectorSIFT(hsv_channels[1]);
    vector<KeyPoint> remainingSIFT_V = vectorSIFT(hsv_channels[2]);

    vector<KeyPoint> remainingSIFT_bgH = vectorSIFT(hsv_bg_channels[0]);
    vector<KeyPoint> remainingSIFT_bgS = vectorSIFT(hsv_bg_channels[1]);
    vector<KeyPoint> remainingSIFT_bgV = vectorSIFT(hsv_bg_channels[2]);

    // draw
    drawKeypoints(img, remainingSIFT, kpImg, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    drawKeypoints(bg, remainingSIFT_bg, kpImg_bg, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    drawKeypoints(img, remainingSIFT_H, kpH, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    drawKeypoints(img, remainingSIFT_S, kpS, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    drawKeypoints(img, remainingSIFT_V, kpV, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    drawKeypoints(bg, remainingSIFT_bgH, kpbgH, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    drawKeypoints(bg, remainingSIFT_bgS, kpbgS, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    drawKeypoints(bg, remainingSIFT_bgV, kpbgV, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    //imshow("kpImg", kpImg);
    //imshow("kpImg_bg", kpImg_bg);
    imshow("kpH", kpH);
    imshow("kpS", kpS);
    imshow("kpV", kpV);
    imshow("kpbgH", kpbgH);
    imshow("kpbgS", kpbgS);
    imshow("kpbgV", kpbgV);
  }

  waitKey(0);
	return 0;
}

vector<KeyPoint> vectorSIFT(Mat img) {
  // start SIFT analysis
  int numSIFT;
  int numSIFT_lowc;

  // default is 0.04 for contrast threshold
  SiftFeatureDetector detector(0,3,0.04,10,1.6);
  SiftFeatureDetector detector_lowc(0,3,0.01,10,1.6);

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

  //return numSIFT - numSIFT_lowc;
  return 1-((float)numSIFT/(float)numSIFT_lowc);
}

void showHSV(Mat in, vector<Mat>& channels){
	// Color Range => H: 0 to 179, S: 0 to 255, V: 0 to 255
	std::string hsv_labels[3] = {"[H]ue", "[S]aturation", "[V]alue"};
	double std_values[3][3] = {{1, 255, 255}, {179, 1, 255}, {179, 0, 1}};
	//showChannels(in, channels, hsv_labels, std_values, COLOR_HSV2BGR);
}

void showChannels(Mat in, vector<Mat>& channel, std::string labels[], double scalar[][3], int convertBack){
	Mat aux[3];
	Mat outputImage = Mat(in.rows, in.cols, in.type());
	for(int i = 0; i < 3; i++){
		// fill the base Mat with standard values based on CS143 from Brown
		aux[0] = Mat::ones(in.rows, in.cols, CV_8UC1) * scalar[i][0];
		aux[1] = Mat::ones(in.rows, in.cols, CV_8UC1) * scalar[i][1];
		aux[2] = Mat::ones(in.rows, in.cols, CV_8UC1) * scalar[i][2];
		aux[i] = channel[i];

		// merge the channels back together
		merge(aux, 3, outputImage);

		// convert from output color space to BGR
		cvtColor(outputImage, outputImage, convertBack);
    imshow(labels[i] + " (3-Chnnels)", outputImage);

		// show single-channel image
		outputImage = channel[i];
		imshow(labels[i] + " (1-Channel)", outputImage);
	}
}
