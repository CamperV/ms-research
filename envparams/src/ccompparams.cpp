// ---
// Jay Danner
// ---

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "ConnCompGroup.h"

using namespace std;
using namespace cv;

float fgAvgPerim(const ConnCompGroup& fg);

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

  ConnCompGroup ccomp(img);
  float avgPerim = fgAvgPerim(ccomp);

  int numCComps = ccomp.comps.size();


#ifdef DEBUG
  cout << "Average Perimeter: " << avgPerim << "\tNum. of CComps: " << numCComps << endl;
#endif
#ifndef DEBUG
  cout << avgPerim << "," << numCComps << endl;
#endif
	return 0;
}

float fgAvgPerim(const ConnCompGroup& fg) {
	float avgPerim = 0;

	for (int i = 0; i < (int) fg.comps.size(); ++i) {
		avgPerim += fg.comps[i].contours[0].size();
	}

	if (fg.comps.size() > 0) {
		avgPerim /= fg.comps.size();
	}

	return avgPerim;
}
