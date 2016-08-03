/**
  * Metrics designed to quantitatively evaluate results of shadow detection
  **/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

double calcDetectRate(Mat mask, Mat shadows);
double calcDiscrimRate(Mat mask, Mat shadows);
