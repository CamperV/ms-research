// ---
// Jay Danner
//
// This program runs evnparameters on foreground objects only.
// NOTE: run with the ../shadowdetection/samples instead!
// ---

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"

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

Scalar frameAvgColorShift(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& shadows);
float frameAvgRGShift(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& shadows);

float ratioSIFT(Mat img, float contrastRatio);

int main(int argc, char** argv) {

  if(argc > 3) {
    cout << "\n'Oak's words echoed... there is a time and a place for everything!' Run with $shadowdetection samples!" << endl;
    cout << "\n---" << endl;
    cout << "> Opening image " << argv[1] << "..." << endl;
    cout << "> Opening background " << argv[2] << "..." << endl;
    cout << "> Opening shadows " << argv[3] << "..." << endl;
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

  Mat orig_shadows = shadows.clone();

  // thresh shadows to fg objects
  threshold(shadows, shadows, 0, 255, THRESH_BINARY);

  Mat imgHSV, bgHSV;
  cvtColor(img, imgHSV, CV_RGB2HSV);
  cvtColor(bg, bgHSV, CV_RGB2HSV);

  vector<Mat> hsv_channels;
  split(imgHSV, hsv_channels);

  vector<Mat> hsv_bg_channels;
  split(bgHSV, hsv_bg_channels);

  float rS = ratioSIFT(hsv_channels[1], 0.01);
  float bgrS = ratioSIFT(hsv_bg_channels[1], 0.01);

  /* PROCESSING */

  //float avgatten = frameAvgAttenuation(imgHSV, bgHSV, shadows);
  //float avgattenall = frameAvgAttenuationAll(imgHSV, bgHSV, shadows);

  //float avgattenhsv = frameAvgAttenuationHSV(img, bg, shadows);
  //float avgattenhsp = frameAvgAttenuationHSP(img, bg, shadows);
  //float avgattenhsi = frameAvgAttenuationHSI(img, bg, shadows);
  //float avgattenhsl = frameAvgAttenuationHSL(img, bg, shadows);
  //float avgattenluma = frameAvgAttenuationLuma(img, bg, shadows);
  //float avgattenw3c = frameAvgAttenuationW3C(img, bg, shadows);
  //float avgattennorm = frameAvgAttenuationNorm(img, bg, shadows);

  float avgattenHSV_RGB = frameAvgAttenuationHSV_RGB(img, bg, shadows);
  //float avgattenHSP_RGB = frameAvgAttenuationHSP_RGB(img, bg, shadows);
  //float avgattenHSI_RGB = frameAvgAttenuationHSI_RGB(img, bg, shadows);
  //float avgattenHSL_RGB = frameAvgAttenuationHSL_RGB(img, bg, shadows);
  //float avgattenluma_RGB = frameAvgAttenuationLuma_RGB(img, bg, shadows);
  //float avgattenw3c_RGB = frameAvgAttenuationW3C_RGB(img, bg, shadows);
  //float avgattennorm_RGB = frameAvgAttenuationNorm_RGB(img, bg, shadows);

  Scalar rgbShift = frameAvgColorShift(img, bg, orig_shadows);
  float rgShift = frameAvgRGShift(img, bg, orig_shadows);

  float siftMult = (float)rS/(float)bgrS;

  //cerr << (float)avgattenhsv << "," << rgShift - (float)rgbShift[0] << endl;
  //cerr << (float)avgattenhsp << "," << rgShift - (float)rgbShift[0] << endl;
  //cerr << (float)avgattenHSV_RGB << "," << rgShift - (float)rgbShift[0] << endl;
  cerr << (float)avgattenHSV_RGB*(float)siftMult << "," << rgShift - (float)rgbShift[0] << endl;
  //cerr << (float)avgattenHSP_RGB << "," << rgShift - (float)rgbShift[0] << endl;
  //cerr << (float)avgattenHSI_RGB << "," << rgShift - (float)rgbShift[0] << endl;
  //cerr << (float)avgattenHSL_RGB << "," << rgShift - (float)rgbShift[0] << endl;
  //cerr << (float)avgattenluma_RGB << "," << rgShift - (float)rgbShift[0] << endl;
  //cerr << (float)avgattennorm_RGB << "," << rgShift - (float)rgbShift[0] << endl;

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
  float stdDev = 0;
  float magdiff = 0;
  vector<float> data;
  vector<float> data_rgb;

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
        data.push_back(1.0/(double)atten);
        data_rgb.push_back(frBrightness/255.0);
			}
		}
	}

  // calculate mean
  for(int i = 0; i < data.size(); i++) {
    avgAtten += data[i];
  }
  avgAtten /= data.size();

  // calculate mean rgb diff
  // %rgb shift (x param)
  for(int i = 0; i < data_rgb.size(); i++) {
    magdiff += data_rgb[i];
  }
  magdiff /= data_rgb.size();

  // calculate stddev
  for(int i = 0; i < data.size(); i++) {
    stdDev += pow(data[i]-avgAtten, 2);
  }
  stdDev = sqrt(stdDev/(float)data.size());

  // 1-%hsv shift (y param)
  float invAvgAtten = 1.0 - avgAtten;
  float invStdDev = 1.0 - stdDev;
  float invMagdiff = 1.0 - magdiff;

  // relating equation (coneR1_brightness.ods)
  double shift_amount = (-0.1130769139)*log(magdiff) - (0.1883942004);

  //return magdiff*avgAtten;
	//return 1.0-avgAtten;
  //return invAvgAtten + shift_amount;
  //return invMagdiff*(invAvgAtten + shift_amount);
  return (invMagdiff*invAvgAtten) + shift_amount;
  //return invAvgAtten + (shift_amount / invMagdiff);
  //return invAvgAtten + (shift_amount / magdifon latexf);
}

float frameAvgAttenuationHSP(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg) {
	float avgAtten = 0;
  float stdDev = 0;
  float magdiff = 0;
  vector<float> data;
  vector<float> data_rgb;

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
        data.push_back(1.0/(double)atten);
        data_rgb.push_back(frBrightness/255.0);
			}
		}
	}

  // calculate mean
  for(int i = 0; i < data.size(); i++) {
    avgAtten += data[i];
  }
  avgAtten /= data.size();

  // calculate mean rgb diff
  // %rgb shift (x param)
  for(int i = 0; i < data_rgb.size(); i++) {
    magdiff += data_rgb[i];
  }
  magdiff /= data_rgb.size();

  // calculate stddev
  for(int i = 0; i < data.size(); i++) {
    stdDev += pow(data[i]-avgAtten, 2);
  }
  stdDev = sqrt(stdDev/(float)data.size());

  // 1-%hsv shift (y param)
  float invAvgAtten = 1.0 - avgAtten;
  float invStdDev = 1.0 - stdDev;
  float invMagdiff = 1.0 - magdiff;

  // relating equation (coneR1_brightness.ods)
  double shift_amount = (-0.1130769139)*log(magdiff) - (0.1883942004);

  //return (invMagdiff*invAvgAtten) + shift_amount;
  return invAvgAtten;
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
  float magdiff = 0;
  float stdDev = 0;
  vector<float> data;
  vector<float> data_rgb;

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

			if (fgPtr[x] > 0 && atten > 0.0) {
        data.push_back(atten);
        data_rgb.push_back(frBrightness/255.0);
			}
		}
	}

  // calculate mean
  for(int i = 0; i < data.size(); i++) {
    avgAtten += data[i];
  }
  avgAtten /= data.size();

  // calculate mean rgb diff
  // %rgb shift (x param)
  for(int i = 0; i < data_rgb.size(); i++) {
    magdiff += data_rgb[i];
  }
  magdiff /= data_rgb.size();

  // calculate stddev
  for(int i = 0; i < data.size(); i++) {
    stdDev += pow(data[i]-avgAtten, 2);
  }
  stdDev = sqrt(stdDev/(float)data.size());

  // 1-%hsv shift (y param)
  float invAvgAtten = 1.0 - avgAtten;
  float invStdDev = 1.0 - stdDev;
  float invMagdiff = 1.0 - magdiff;

  // relating equation (coneR1_brightness.ods)
  double shift_amount = (-0.0758149245)*log(magdiff) - (0.149215643);

  //return magdiff*avgAtten;
	//return 1.0-avgAtten;
  //return invAvgAtten + shift_amount;
  //return invMagdiff*(invAvgAtten + shift_amount);
  return (invMagdiff*invAvgAtten) + shift_amount;
  //return invAvgAtten + (shift_amount / invMagdiff);
  //return invAvgAtten + (shift_amount / magdiff);

}

float frameAvgAttenuationHSP_RGB(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg) {
	float avgAtten = 0;
  float magdiff = 0;
  float stdDev = 0;
  vector<float> data;
  vector<float> data_rgb;

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

			//double frBrightness = sqrt( .241*frToBgVec[0]*frToBgVec[0] +
      //                            .691*frToBgVec[1]*frToBgVec[1] +
      //                            .068*frToBgVec[2]*frToBgVec[2]  );
			//double bgBrightness = sqrt( .241*bgVec[0]*bgVec[0] +
      //                            .691*bgVec[1]*bgVec[1] +
      //                            .068*bgVec[2]*bgVec[2]  );
			double frBrightness = sqrt( .299*frToBgVec[0]*frToBgVec[0] +
                                  .587*frToBgVec[1]*frToBgVec[1] +
                                  .114*frToBgVec[2]*frToBgVec[2]  );
			double bgBrightness = sqrt( .299*bgVec[0]*bgVec[0] +
                                  .587*bgVec[1]*bgVec[1] +
                                  .114*bgVec[2]*bgVec[2]  );

			double atten = std::min(frBrightness / bgBrightness, 1.0);

			if (fgPtr[x] > 0 && atten > 0.0) {
        data.push_back(atten);
        data_rgb.push_back(frBrightness/255.0);
			}
		}
	}

  // calculate mean
  for(int i = 0; i < data.size(); i++) {
    avgAtten += data[i];
  }
  avgAtten /= data.size();

  // calculate mean rgb diff
  // %rgb shift (x param)
  for(int i = 0; i < data_rgb.size(); i++) {
    magdiff += data_rgb[i];
  }
  magdiff /= data_rgb.size();

  // calculate stddev
  for(int i = 0; i < data.size(); i++) {
    stdDev += pow(data[i]-avgAtten, 2);
  }
  stdDev = sqrt(stdDev/(float)data.size());

  // 1-%hsv shift (y param)
  float invAvgAtten = 1.0 - avgAtten;
  float invStdDev = 1.0 - stdDev;
  float invMagdiff = 1.0 - magdiff;

  // relating equation (coneR1_brightness.ods)
  double shift_amount = (-0.101750886)*log(magdiff) - (0.225961276);

  return (invMagdiff*invAvgAtten) + shift_amount;
}

float frameAvgAttenuationHSI_RGB(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg) {
	float avgAtten = 0;
  float magdiff = 0;
  float stdDev = 0;
  vector<float> data;
  vector<float> data_rgb;

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

			if (fgPtr[x] > 0 && atten > 0.0) {
        data.push_back(atten);
        data_rgb.push_back(frBrightness/255.0);
			}
		}
	}

  // calculate mean
  for(int i = 0; i < data.size(); i++) {
    avgAtten += data[i];
  }
  avgAtten /= data.size();

  // calculate mean rgb diff
  // %rgb shift (x param)
  for(int i = 0; i < data_rgb.size(); i++) {
    magdiff += data_rgb[i];
  }
  magdiff /= data_rgb.size();

  // calculate stddev
  for(int i = 0; i < data.size(); i++) {
    stdDev += pow(data[i]-avgAtten, 2);
  }
  stdDev = sqrt(stdDev/(float)data.size());

  // 1-%hsv shift (y param)
  float invAvgAtten = 1.0 - avgAtten;
  float invStdDev = 1.0 - stdDev;
  float invMagdiff = 1.0 - magdiff;

  // relating equation (coneR1_brightness.ods)
  double shift_amount = (-0.0886472287)*log(magdiff) - (0.2073675537);

  return (invMagdiff*invAvgAtten) + shift_amount;
}

float frameAvgAttenuationHSL_RGB(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg) {
	float avgAtten = 0;
  float magdiff = 0;
  float stdDev = 0;
  vector<float> data;
  vector<float> data_rgb;

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

			if (fgPtr[x] > 0 && atten > 0.0) {
        data.push_back(atten);
        data_rgb.push_back(frBrightness/255.0);
			}
		}
	}

  // calculate mean
  for(int i = 0; i < data.size(); i++) {
    avgAtten += data[i];
  }
  avgAtten /= data.size();

  // calculate mean rgb diff
  // %rgb shift (x param)
  for(int i = 0; i < data_rgb.size(); i++) {
    magdiff += data_rgb[i];
  }
  magdiff /= data_rgb.size();

  // calculate stddev
  for(int i = 0; i < data.size(); i++) {
    stdDev += pow(data[i]-avgAtten, 2);
  }
  stdDev = sqrt(stdDev/(float)data.size());

  // 1-%hsv shift (y param)
  float invAvgAtten = 1.0 - avgAtten;
  float invStdDev = 1.0 - stdDev;
  float invMagdiff = 1.0 - magdiff;

  // relating equation (coneR1_brightness.ods)
  double shift_amount = (-0.0904166328)*log(magdiff) - (0.2130357418);

  return (invMagdiff*invAvgAtten) + shift_amount;
}

float frameAvgAttenuationLuma_RGB(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& fg) {
	float avgAtten = 0;
  float magdiff = 0;
  float stdDev = 0;
  vector<float> data;
  vector<float> data_rgb;

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

			double atten = std::min(frBrightness / bgBrightness, 1.0);

			if (fgPtr[x] > 0 && atten > 0.0) {
        data.push_back(atten);
        data_rgb.push_back(frBrightness/255.0);
			}
		}
	}

  // calculate mean
  for(int i = 0; i < data.size(); i++) {
    avgAtten += data[i];
  }
  avgAtten /= data.size();

  // calculate mean rgb diff
  // %rgb shift (x param)
  for(int i = 0; i < data_rgb.size(); i++) {
    magdiff += data_rgb[i];
  }
  magdiff /= data_rgb.size();

  // calculate stddev
  for(int i = 0; i < data.size(); i++) {
    stdDev += pow(data[i]-avgAtten, 2);
  }
  stdDev = sqrt(stdDev/(float)data.size());

  // 1-%hsv shift (y param)
  float invAvgAtten = 1.0 - avgAtten;
  float invStdDev = 1.0 - stdDev;
  float invMagdiff = 1.0 - magdiff;

  // relating equation (coneR1_brightness.ods)
  double shift_amount = (-0.0824515512)*log(magdiff) - (0.188114743);

  return (invMagdiff*invAvgAtten) + shift_amount;
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
  float magdiff = 0;
  float stdDev = 0;
  vector<float> data;
  vector<float> data_rgb;

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

			if (fgPtr[x] > 0 && atten > 0.0) {
        data.push_back(atten);
        data_rgb.push_back(frBrightness/255.0);
			}
		}
	}

  // calculate mean
  for(int i = 0; i < data.size(); i++) {
    avgAtten += data[i];
  }
  avgAtten /= data.size();

  // calculate mean rgb diff
  // %rgb shift (x param)
  for(int i = 0; i < data_rgb.size(); i++) {
    magdiff += data_rgb[i];
  }
  magdiff /= data_rgb.size();

  // calculate stddev
  for(int i = 0; i < data.size(); i++) {
    stdDev += pow(data[i]-avgAtten, 2);
  }
  stdDev = sqrt(stdDev/(float)data.size());

  // 1-%hsv shift (y param)
  float invAvgAtten = 1.0 - avgAtten;
  float invStdDev = 1.0 - stdDev;
  float invMagdiff = 1.0 - magdiff;

  // relating equation (coneR1_brightness.ods)
  double shift_amount = (-0.1068870251)*log(magdiff) - (0.1818029717);

  return (invMagdiff*invAvgAtten) + shift_amount;
}

Scalar frameAvgColorShift(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& shadows) {
  Scalar rgbShift = Scalar(0,0,0);
	int count = 0;

	for (int y = 0; y < frame.rows; ++y) {
		const uchar* fgPtr = shadows.ptr(y);
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

			if (fgPtr[x] == 127) {
        rgbShift += frToBgVec;
				++count;
			}
		}
	}

	if (count > 0) {
		rgbShift /= count;
	}

	return rgbShift;
}

float frameAvgRGShift(const cv::Mat& frame, const cv::Mat& bg, const cv::Mat& shadows) {
  Scalar rgbShift = Scalar(0,0,0);
	int count = 0;

	for (int y = 0; y < frame.rows; ++y) {
		const uchar* fgPtr = shadows.ptr(y);
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

      //float rgShift = (frToBgVec[1]+frToBgVec[2])/2.0;

			if (fgPtr[x] == 127) {
        rgbShift += frToBgVec;
				++count;
			}
		}
	}

	if (count > 0) {
		rgbShift /= count;
	}
  float rgShift = (rgbShift[1]+rgbShift[2])/2.0;

	return rgShift;
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
