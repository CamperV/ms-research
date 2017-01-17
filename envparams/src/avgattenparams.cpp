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

float frameAvgAttenuation(const cv::Mat& hsvFrame, const cv::Mat& hsvBg, const cv::Mat& fg);

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

  Mat imgHSV, bgHSV;
  cvtColor(img, imgHSV, CV_RGB2HSV);
  cvtColor(bg, bgHSV, CV_RGB2HSV);

  /* PROCESSING */

  float avgatten = frameAvgAttenuation(imgHSV, bgHSV, shadows);

#ifdef DEBUG
  cout << "Avg Atten: " << avgatten << endl;
#endif
#ifndef DEBUG
  cout << avgatten << endl;
#endif

	return 0;
}

float frameAvgAttenuation(const cv::Mat& hsvFrame, const cv::Mat& hsvBg, const cv::Mat& fg) {
	float avgAtten = 0;
	int count = 0;
	for (int y = 0; y < hsvFrame.rows; ++y) {
		const uchar* fgPtr = fg.ptr(y);
		const uchar* framePtr = hsvFrame.ptr(y);
		const uchar* bgPtr = hsvBg.ptr(y);

		for (int x = 0; x < hsvFrame.cols; ++x) {
			if (fgPtr[x] > 0) {
				float atten = (float) (10 + bgPtr[x * 3 + 2]) / (10 + framePtr[x * 3 + 2]);
				bool vIsShadow = (atten > 1 && atten < 5);

				int hDiff = CV_IABS(framePtr[x * 3] - bgPtr[x * 3]);
				if (hDiff > 90) {
					hDiff = 180 - hDiff;
				}
				bool hIsShadow = (hDiff < 4);

				if (vIsShadow && hIsShadow) {
					avgAtten += atten;
					++count;
				}
			}
		}
	}

	if (count > 0) {
		avgAtten /= count;
	}

	return avgAtten;
}
