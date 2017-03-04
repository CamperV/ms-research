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
float frameAvgAttenuationAll(const cv::Mat& hsvFrame, const cv::Mat& hsvBg, const cv::Mat& fg);

float frameAvgAttenuationHSV(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg);
float frameAvgAttenuationHSP(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg);
float frameAvgAttenuationHSI(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg);
float frameAvgAttenuationHSL(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg);
float frameAvgAttenuationLuma(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg);
float frameAvgAttenuationW3C(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg);
float frameAvgAttenuationNorm(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg);

float frameAvgAttenuationHSV_RGB(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg);
float frameAvgAttenuationHSP_RGB(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg);
float frameAvgAttenuationHSI_RGB(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg);
float frameAvgAttenuationHSL_RGB(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg);
float frameAvgAttenuationLuma_RGB(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg);
float frameAvgAttenuationW3C_RGB(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg);
float frameAvgAttenuationNorm_RGB(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg);

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

  //float avgatten = frameAvgAttenuation(imgHSV, bgHSV, shadows);
  //float avgattenall = frameAvgAttenuationAll(imgHSV, bgHSV, shadows);
//
  //float avgattenhsv = frameAvgAttenuationHSV(img, bg, shadows);
  //float avgattenhsp = frameAvgAttenuationHSP(img, bg, shadows);
  //float avgattenhsi = frameAvgAttenuationHSI(img, bg, shadows);
  //float avgattenhsl = frameAvgAttenuationHSL(img, bg, shadows);
  //float avgattenluma = frameAvgAttenuationLuma(img, bg, shadows);
  //float avgattenw3c = frameAvgAttenuationW3C(img, bg, shadows);
  //float avgattennorm = frameAvgAttenuationNorm(img, bg, shadows);

  float avgattenHSV_RGB = frameAvgAttenuationHSV_RGB(img, bg, shadows);
  float avgattenHSP_RGB = frameAvgAttenuationHSP_RGB(img, bg, shadows);
  float avgattenHSI_RGB = frameAvgAttenuationHSI_RGB(img, bg, shadows);
  float avgattenHSL_RGB = frameAvgAttenuationHSL_RGB(img, bg, shadows);
  float avgattenluma_RGB = frameAvgAttenuationLuma_RGB(img, bg, shadows);
  float avgattenw3c_RGB = frameAvgAttenuationW3C_RGB(img, bg, shadows);
  float avgattennorm_RGB = frameAvgAttenuationNorm_RGB(img, bg, shadows);

  //cout << avgattenHSP_RGB << endl;
  cout << avgattenHSV_RGB << ","
       << avgattenHSP_RGB << ","
       << avgattenHSI_RGB << ","
       << avgattenHSL_RGB << ","
       << avgattenluma_RGB << ","
       << avgattenw3c_RGB << ","
       << avgattennorm_RGB
       << endl;

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

float frameAvgAttenuationAll(const cv::Mat& hsvFrame, const cv::Mat& hsvBg, const cv::Mat& fg) {
	float avgAtten = 0;
	int count = 0;
	for (int y = 0; y < hsvFrame.rows; ++y) {
		const uchar* fgPtr = fg.ptr(y);
		const uchar* framePtr = hsvFrame.ptr(y);
		const uchar* bgPtr = hsvBg.ptr(y);

		for (int x = 0; x < hsvFrame.cols; ++x) {
			if (fgPtr[x] > 0) {
				float atten = (float) (10 + bgPtr[x * 3 + 2]) / (10 + framePtr[x * 3 + 2]);
				avgAtten += atten;
				++count;
			}
		}
	}

	if (count > 0) {
		avgAtten /= count;
	}

	return avgAtten;
}

float frameAvgAttenuationHSV(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg) {
	float avgAtten = 0;
	int count = 0;

	for (int y = 0; y < frame.rows; ++y) {
		const uchar* fgPtr = fg.ptr(y);
		const uchar* framePtr = frame.ptr(y);
		const uchar* bgPtr = bg.ptr(y);
		for (int x = 0; x < frame.cols; ++x) {

			Scalar frToBgVec( framePtr[x*3],
                        framePtr[x*3+1],
                        framePtr[x*3+2]);
			Scalar bgVec(     bgPtr[x*3],
                        bgPtr[x*3+1],
                        bgPtr[x*3+2]);

			double frBrightness = max((double)frToBgVec[0], (double)frToBgVec[1]);
			frBrightness = max((double)frToBgVec[2], frBrightness);

			double bgBrightness = max((double)bgVec[0], (double)bgVec[1]);
			bgBrightness = max((double)bgVec[2], bgBrightness);

			double atten = (10+bgBrightness)/(10+frBrightness);

			if (fgPtr[x] > 0 && atten > 1.0) {
				avgAtten += atten;
				++count;
			}
		}
	}

	if (count > 0) {
		avgAtten /= count;
	}

	return avgAtten;
}

float frameAvgAttenuationHSP(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg) {
	float avgAtten = 0;
	int count = 0;

	for (int y = 0; y < frame.rows; ++y) {
		const uchar* fgPtr = fg.ptr(y);
		const uchar* framePtr = frame.ptr(y);
		const uchar* bgPtr = bg.ptr(y);
		for (int x = 0; x < frame.cols; ++x) {

      // use RGB to calculate brightness
      // brightness bg/fg
			Scalar frVec( framePtr[x*3],
                    framePtr[x*3+1],
                    framePtr[x*3+2]);
			Scalar bgVec( bgPtr[x*3],
                    bgPtr[x*3+1],
                    bgPtr[x*3+2]);

			double frBrightness = sqrt( .299*frVec[0]*frVec[0] +
                                  .587*frVec[1]*frVec[1] +
                                  .114*frVec[2]*frVec[2]  );
			double bgBrightness = sqrt( .299*bgVec[0]*bgVec[0] +
                                  .587*bgVec[1]*bgVec[1] +
                                  .114*bgVec[2]*bgVec[2]  );

      double atten = (10+bgBrightness)/(10+frBrightness);

			if (fgPtr[x] > 0 && atten > 1.0) {
				avgAtten += atten;
				++count;
			}
		}
	}

	if (count > 0) {
		avgAtten /= count;
	}

	return avgAtten;
}

float frameAvgAttenuationHSI(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg) {
	float avgAtten = 0;
	int count = 0;

	for (int y = 0; y < frame.rows; ++y) {
		const uchar* fgPtr = fg.ptr(y);
		const uchar* framePtr = frame.ptr(y);
		const uchar* bgPtr = bg.ptr(y);
		for (int x = 0; x < frame.cols; ++x) {

			Scalar frVec( framePtr[x*3],
                        framePtr[x*3+1],
                        framePtr[x*3+2]);
			Scalar bgVec(     bgPtr[x*3],
                        bgPtr[x*3+1],
                        bgPtr[x*3+2]);

			double frBrightness = ((double)frVec[0]+(double)frVec[1]+(double)frVec[2])/3.0;
			double bgBrightness = ((double)bgVec[0]+(double)bgVec[1]+(double)bgVec[2])/3.0;

			double atten = (10+bgBrightness)/(10+frBrightness);

			//if (fgPtr[x] > 0) {
			if (fgPtr[x] > 0 && atten > 1.0) {
				avgAtten += atten;
				++count;
			}
		}
	}

	if (count > 0) {
		avgAtten /= count;
	}

	return avgAtten;
}

float frameAvgAttenuationHSL(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg) {
	float avgAtten = 0;
	int count = 0;

	for (int y = 0; y < frame.rows; ++y) {
		const uchar* fgPtr = fg.ptr(y);
		const uchar* framePtr = frame.ptr(y);
		const uchar* bgPtr = bg.ptr(y);
		for (int x = 0; x < frame.cols; ++x) {

			Scalar frToBgVec( framePtr[x*3],
                        framePtr[x*3+1],
                        framePtr[x*3+2]);
			Scalar bgVec(     bgPtr[x*3],
                        bgPtr[x*3+1],
                        bgPtr[x*3+2]);

			double frBrightnessMax = max((double)frToBgVec[0], (double)frToBgVec[1]);
			frBrightnessMax = max((double)frToBgVec[2], frBrightnessMax);
			double frBrightnessMin = min((double)frToBgVec[0], (double)frToBgVec[1]);
			frBrightnessMin = min((double)frToBgVec[2], frBrightnessMin);
      double frBrightness = (frBrightnessMax + frBrightnessMin) / 2.0;

			double bgBrightnessMax = max((double)bgVec[0], (double)bgVec[1]);
			bgBrightnessMax = max((double)bgVec[2], bgBrightnessMax);
			double bgBrightnessMin = min((double)bgVec[0], (double)bgVec[1]);
			bgBrightnessMin = min((double)bgVec[2], bgBrightnessMin);
      double bgBrightness = (bgBrightnessMax + bgBrightnessMin) / 2.0;

			double atten = (10+bgBrightness)/(10+frBrightness);

			//if (fgPtr[x] > 0) {
			if (fgPtr[x] > 0 && atten > 1.0) {
				avgAtten += atten;
				++count;
			}
		}
	}

	if (count > 0) {
		avgAtten /= count;
	}

	return avgAtten;
}

float frameAvgAttenuationLuma(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg) {
	float avgAtten = 0;
	int count = 0;

	for (int y = 0; y < frame.rows; ++y) {
		const uchar* fgPtr = fg.ptr(y);
		const uchar* framePtr = frame.ptr(y);
		const uchar* bgPtr = bg.ptr(y);
		for (int x = 0; x < frame.cols; ++x) {

			Scalar frToBgVec( framePtr[x*3],
                        framePtr[x*3+1],
                        framePtr[x*3+2]);
			Scalar bgVec(     bgPtr[x*3],
                        bgPtr[x*3+1],
                        bgPtr[x*3+2]);

      double frBrightness = .299*(double)frToBgVec[0] + .587*(double)frToBgVec[1] + .114*(double)frToBgVec[2];
      double bgBrightness = .299*(double)bgVec[0] + .587*(double)bgVec[1] + .114*(double)bgVec[2];

			double atten = (10+bgBrightness)/(10+frBrightness);

			//if (fgPtr[x] > 0) {
			if (fgPtr[x] > 0 && atten > 1.0) {
				avgAtten += atten;
				++count;
			}
		}
	}

	if (count > 0) {
		avgAtten /= count;
	}

	return avgAtten;
}

float frameAvgAttenuationW3C(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg) {
	float avgAtten = 0;
	int count = 0;

	for (int y = 0; y < frame.rows; ++y) {
		const uchar* fgPtr = fg.ptr(y);
		const uchar* framePtr = frame.ptr(y);
		const uchar* bgPtr = bg.ptr(y);
		for (int x = 0; x < frame.cols; ++x) {

			Scalar frToBgVec( framePtr[x*3],
                        framePtr[x*3+1],
                        framePtr[x*3+2]);
			Scalar bgVec(     bgPtr[x*3],
                        bgPtr[x*3+1],
                        bgPtr[x*3+2]);

      double frBrightness = .299*(double)frToBgVec[0] + .587*(double)frToBgVec[1] + .114*(double)frToBgVec[2];
      double bgBrightness = .299*(double)bgVec[0] + .587*(double)bgVec[1] + .114*(double)bgVec[2];

      double rDiff = max(frToBgVec[0], bgVec[0]) - min(frToBgVec[0], bgVec[0]);
      double gDiff = max(frToBgVec[1], bgVec[1]) - min(frToBgVec[1], bgVec[1]);
      double bDiff = max(frToBgVec[2], bgVec[2]) - min(frToBgVec[2], bgVec[2]);

			double atten = rDiff + gDiff + bDiff;

			if (fgPtr[x] > 0) {
				avgAtten += atten;
				++count;
			}
		}
	}

	if (count > 0) {
		avgAtten /= count;
	}

	return avgAtten;
}

float frameAvgAttenuationNorm(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg) {
	float avgAtten = 0;
	int count = 0;

	for (int y = 0; y < frame.rows; ++y) {
		const uchar* fgPtr = fg.ptr(y);
		const uchar* framePtr = frame.ptr(y);
		const uchar* bgPtr = bg.ptr(y);
		for (int x = 0; x < frame.cols; ++x) {

			Scalar frVec(     framePtr[x*3],
                        framePtr[x*3+1],
                        framePtr[x*3+2]);
			Scalar frToBgVec( bgPtr[x*3] -   framePtr[x*3],
                        bgPtr[x*3+1] - framePtr[x*3+1],
                        bgPtr[x*3+2] - framePtr[x*3+2]);
			Scalar bgVec(     bgPtr[x*3],
                        bgPtr[x*3+1],
                        bgPtr[x*3+2]);
			//double frBrightness = cv::norm(frToBgVec);
			double frBrightness = cv::norm(frVec);
			double bgBrightness = cv::norm(bgVec);

			//double atten = std::min(frBrightness / bgBrightness, 1.0);
      double atten = (10+bgBrightness)/(10+frBrightness);

			//if (fgPtr[x] > 0) {
			if (fgPtr[x] > 0 && atten > 1.0) {
				avgAtten += atten;
				++count;
			}
		}
	}

	if (count > 0) {
		avgAtten /= count;
	}

	return avgAtten;
}

float frameAvgAttenuationHSV_RGB(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg) {
	float avgAtten = 0;
	int count = 0;

	for (int y = 0; y < frame.rows; ++y) {
		const uchar* fgPtr = fg.ptr(y);
		const uchar* framePtr = frame.ptr(y);
		const uchar* bgPtr = bg.ptr(y);
		for (int x = 0; x < frame.cols; ++x) {

			Scalar frVec(     framePtr[x*3],
                        framePtr[x*3+1],
                        framePtr[x*3+2]);
			Scalar frToBgVec( bgPtr[x*3] -   framePtr[x*3],
                        bgPtr[x*3+1] - framePtr[x*3+1],
                        bgPtr[x*3+2] - framePtr[x*3+2]);
			Scalar bgVec(     bgPtr[x*3],
                        bgPtr[x*3+1],
                        bgPtr[x*3+2]);


			//double frBrightness = max((double)abs(frToBgVec[0]), (double)abs(frToBgVec[1]));
			//frBrightness = max((double)abs(frToBgVec[2]), abs(frBrightness));

      double frBrightness;

      // this should retain signs
      if( (double)abs(frToBgVec[0]) > (double)abs(frToBgVec[1]) ) frBrightness = frToBgVec[0];
      else frBrightness = frToBgVec[1];
      if( (double)abs(frToBgVec[2]) > (double)abs(frBrightness) ) frBrightness = frToBgVec[2];

			double bgBrightness = max((double)bgVec[0], (double)bgVec[1]);
			bgBrightness = max((double)bgVec[2], bgBrightness);

			double atten = std::min(frBrightness / bgBrightness, 1.0);
			//double atten = std::min((bgBrightness - frBrightness) / bgBrightness, 1.0);

			if (fgPtr[x] > 0 && atten > 0.0) {
				avgAtten += atten;
				++count;
			}
		}
	}

	if (count > 0) {
		avgAtten /= count;
	}

	return avgAtten;
}

float frameAvgAttenuationHSP_RGB(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg) {
	float avgAtten = 0;
	int count = 0;

	for (int y = 0; y < frame.rows; ++y) {
		const uchar* fgPtr = fg.ptr(y);
		const uchar* framePtr = frame.ptr(y);
		const uchar* bgPtr = bg.ptr(y);
		for (int x = 0; x < frame.cols; ++x) {

			Scalar frVec(     framePtr[x*3],
                        framePtr[x*3+1],
                        framePtr[x*3+2]);
			Scalar frToBgVec( bgPtr[x*3] -   framePtr[x*3],
                        bgPtr[x*3+1] - framePtr[x*3+1],
                        bgPtr[x*3+2] - framePtr[x*3+2]);
			Scalar bgVec(     bgPtr[x*3],
                        bgPtr[x*3+1],
                        bgPtr[x*3+2]);

			double frBrightness = sqrt( .299*frToBgVec[0]*frToBgVec[0] +
                                  .587*frToBgVec[1]*frToBgVec[1] +
                                  .114*frToBgVec[2]*frToBgVec[2]  );
			double bgBrightness = sqrt( .299*bgVec[0]*bgVec[0] +
                                  .587*bgVec[1]*bgVec[1] +
                                  .114*bgVec[2]*bgVec[2]  );

			double atten = std::min(frBrightness / bgBrightness, 1.0);

			if (fgPtr[x] > 0 && atten > 0.0) {
				avgAtten += atten;
				++count;
			}
		}
	}

	if (count > 0) {
		avgAtten /= count;
	}

	return avgAtten;
}

float frameAvgAttenuationHSI_RGB(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg) {
	float avgAtten = 0;
	int count = 0;

	for (int y = 0; y < frame.rows; ++y) {
		const uchar* fgPtr = fg.ptr(y);
		const uchar* framePtr = frame.ptr(y);
		const uchar* bgPtr = bg.ptr(y);
		for (int x = 0; x < frame.cols; ++x) {

			Scalar frVec(     framePtr[x*3],
                        framePtr[x*3+1],
                        framePtr[x*3+2]);
			Scalar frToBgVec( bgPtr[x*3] -   framePtr[x*3],
                        bgPtr[x*3+1] - framePtr[x*3+1],
                        bgPtr[x*3+2] - framePtr[x*3+2]);
			Scalar bgVec(     bgPtr[x*3],
                        bgPtr[x*3+1],
                        bgPtr[x*3+2]);

			double frBrightness = ((double)frToBgVec[0]+(double)frToBgVec[1]+(double)frToBgVec[2])/3.0;
			double bgBrightness = ((double)bgVec[0]+(double)bgVec[1]+(double)bgVec[2])/3.0;

			double atten = std::min(frBrightness / bgBrightness, 1.0);
			//double atten = std::min((bgBrightness - frBrightness) / bgBrightness, 1.0);

			//if (fgPtr[x] > 0) {
			if (fgPtr[x] > 0 && atten > 0.0) {
				avgAtten += atten;
				++count;
			}
		}
	}

	if (count > 0) {
		avgAtten /= count;
	}

	return avgAtten;
}

float frameAvgAttenuationHSL_RGB(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg) {
	float avgAtten = 0;
	int count = 0;

	for (int y = 0; y < frame.rows; ++y) {
		const uchar* fgPtr = fg.ptr(y);
		const uchar* framePtr = frame.ptr(y);
		const uchar* bgPtr = bg.ptr(y);
		for (int x = 0; x < frame.cols; ++x) {

			Scalar frVec(     framePtr[x*3],
                        framePtr[x*3+1],
                        framePtr[x*3+2]);
			Scalar frToBgVec( bgPtr[x*3] -   framePtr[x*3],
                        bgPtr[x*3+1] - framePtr[x*3+1],
                        bgPtr[x*3+2] - framePtr[x*3+2]);
			Scalar bgVec(     bgPtr[x*3],
                        bgPtr[x*3+1],
                        bgPtr[x*3+2]);

			//double frBrightnessMax = max((double)frToBgVec[0], (double)frToBgVec[1]);
			//frBrightnessMax = max((double)frToBgVec[2], frBrightnessMax);
			//double frBrightnessMin = min((double)frToBgVec[0], (double)frToBgVec[1]);
			//frBrightnessMin = min((double)frToBgVec[2], frBrightnessMin);
      //double frBrightness = (frBrightnessMax + frBrightnessMin) / 2.0;

      double frBrightnessMax, frBrightnessMin;
      // this should retain signs
      // max
      if( (double)abs(frToBgVec[0]) > (double)abs(frToBgVec[1]) ) frBrightnessMax = frToBgVec[0];
      else frBrightnessMax = frToBgVec[1];
      if( (double)abs(frToBgVec[2]) > (double)abs(frBrightnessMax) ) frBrightnessMax = frToBgVec[2];
      // min
      if( (double)abs(frToBgVec[0]) < (double)abs(frToBgVec[1]) ) frBrightnessMin = frToBgVec[0];
      else frBrightnessMin = frToBgVec[1];
      if( (double)abs(frToBgVec[2]) < (double)abs(frBrightnessMin) ) frBrightnessMin = frToBgVec[2];
      double frBrightness = (frBrightnessMax + frBrightnessMin) / 2.0;

			double bgBrightnessMax = max((double)bgVec[0], (double)bgVec[1]);
			bgBrightnessMax = max((double)bgVec[2], bgBrightnessMax);
			double bgBrightnessMin = min((double)bgVec[0], (double)bgVec[1]);
			bgBrightnessMin = min((double)bgVec[2], bgBrightnessMin);
      double bgBrightness = (bgBrightnessMax + bgBrightnessMin) / 2.0;

			double atten = std::min(frBrightness / bgBrightness, 1.0);
			//double atten = std::min((bgBrightness - frBrightness) / bgBrightness, 1.0);

			//if (fgPtr[x] > 0) {
			if (fgPtr[x] > 0 && atten > 0.0) {
				avgAtten += atten;
				++count;
			}
		}
	}

	if (count > 0) {
		avgAtten /= count;
	}

	return avgAtten;
}

float frameAvgAttenuationLuma_RGB(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg) {
	float avgAtten = 0;
	int count = 0;

	for (int y = 0; y < frame.rows; ++y) {
		const uchar* fgPtr = fg.ptr(y);
		const uchar* framePtr = frame.ptr(y);
		const uchar* bgPtr = bg.ptr(y);
		for (int x = 0; x < frame.cols; ++x) {

			Scalar frVec(     framePtr[x*3],
                        framePtr[x*3+1],
                        framePtr[x*3+2]);
			Scalar frToBgVec( bgPtr[x*3] -   framePtr[x*3],
                        bgPtr[x*3+1] - framePtr[x*3+1],
                        bgPtr[x*3+2] - framePtr[x*3+2]);
			Scalar bgVec(     bgPtr[x*3],
                        bgPtr[x*3+1],
                        bgPtr[x*3+2]);

      double frBrightness = .299*(double)frToBgVec[0] + .587*(double)frToBgVec[1] + .114*(double)frToBgVec[2];
      double bgBrightness = .299*(double)bgVec[0] + .587*(double)bgVec[1] + .114*(double)bgVec[2];

			//double atten = (10+bgBrightness)/(10+frBrightness);
			double atten = std::min(frBrightness / bgBrightness, 1.0);

			//if (fgPtr[x] > 0) {
			if (fgPtr[x] > 0 && atten > 0.0) {
				avgAtten += atten;
				++count;
			}
		}
	}

	if (count > 0) {
		avgAtten /= count;
	}

	return avgAtten;
}

float frameAvgAttenuationW3C_RGB(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg) {
	float avgAtten = 0;
	int count = 0;

	for (int y = 0; y < frame.rows; ++y) {
		const uchar* fgPtr = fg.ptr(y);
		const uchar* framePtr = frame.ptr(y);
		const uchar* bgPtr = bg.ptr(y);
		for (int x = 0; x < frame.cols; ++x) {

			Scalar frVec(     framePtr[x*3],
                        framePtr[x*3+1],
                        framePtr[x*3+2]);
			Scalar frToBgVec( bgPtr[x*3] -   framePtr[x*3],
                        bgPtr[x*3+1] - framePtr[x*3+1],
                        bgPtr[x*3+2] - framePtr[x*3+2]);
			Scalar bgVec(     bgPtr[x*3],
                        bgPtr[x*3+1],
                        bgPtr[x*3+2]);

      //double frBrightness = .299*(double)frToBgVec[0] + .587*(double)frToBgVec[1] + .114*(double)frToBgVec[2];
      //double bgBrightness = .299*(double)bgVec[0] + .587*(double)bgVec[1] + .114*(double)bgVec[2];

      double rDiff = max(frToBgVec[0], bgVec[0]) - min(frToBgVec[0], bgVec[0]);
      double gDiff = max(frToBgVec[1], bgVec[1]) - min(frToBgVec[1], bgVec[1]);
      double bDiff = max(frToBgVec[2], bgVec[2]) - min(frToBgVec[2], bgVec[2]);

			double atten = rDiff + gDiff + bDiff;

			if (fgPtr[x] > 0) {
			//if (fgPtr[x] > 0 && atten > 0.0) {
				avgAtten += atten;
				++count;
			}
		}
	}

	if (count > 0) {
		avgAtten /= count;
	}

	return avgAtten;
}

float frameAvgAttenuationNorm_RGB(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg) {
	float avgAtten = 0;
	int count = 0;

	for (int y = 0; y < frame.rows; ++y) {
		const uchar* fgPtr = fg.ptr(y);
		const uchar* framePtr = frame.ptr(y);
		const uchar* bgPtr = bg.ptr(y);
		for (int x = 0; x < frame.cols; ++x) {

			Scalar frVec(     framePtr[x*3],
                        framePtr[x*3+1],
                        framePtr[x*3+2]);
			Scalar frToBgVec( bgPtr[x*3] -   framePtr[x*3],
                        bgPtr[x*3+1] - framePtr[x*3+1],
                        bgPtr[x*3+2] - framePtr[x*3+2]);
			Scalar bgVec(     bgPtr[x*3],
                        bgPtr[x*3+1],
                        bgPtr[x*3+2]);
			double frBrightness = cv::norm(frToBgVec);
			//double frBrightness = cv::norm(frVec);
			double bgBrightness = cv::norm(bgVec);

			double atten = std::min(frBrightness / bgBrightness, 1.0);
      //double atten = (10+bgBrightness)/(10+frBrightness);

			//if (fgPtr[x] > 0) {
			if (fgPtr[x] > 0 && atten > 0.0) {
				avgAtten += atten;
				++count;
			}
		}
	}

	if (count > 0) {
		avgAtten /= count;
	}

	return avgAtten;
}
