// ---
// Jay Danner
// ---

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
  
  string infile = "";
  Mat img;

  if(argc > 1) {
    cout << "Opening image " << argv[1] << "..." << endl;
    infile = argv[1];
  } else {
    cout << "No input file specified: using DEFAULT." << endl;
    infile = "samples/9am_4-3.jpg";
  }

  img = imread(infile, CV_LOAD_IMAGE_COLOR);

  if (!img.data) { 
    cout << "Image failed to open." << endl; 
    return -1; 
  }

  // init windows
  namedWindow("Image 1", 1);

  // preprocess
  Mat imgHSV;
  cvtColor(img, imgHSV, CV_RGB2HSV);

  /* PROCESSING */

  /*
   * Possible environmental parameters:
   * (i) Texture density
   * (ii) Illumination
   * (iii) average chromacity
   * (iv) contrast
   **/

   vector<Mat> channels(3);
   split(imgHSV, channels);

   Scalar hmean, smean, vmean;
   hmean = mean(channels[0]);
   smean = mean(channels[1]);
   vmean = mean(channels[2]);
   
   cout << "Avg Hue:\t" << hmean[0] << endl;
   cout << "Avg Saturation:\t" << smean[0] << endl;
   cout << "Avg Value:\t" << vmean[0] << endl;

  for(;;) {
	  // show results
	  imshow("Image 1", img);
    if(waitKey(0) == 27) break;
  }
	return 0;
}
