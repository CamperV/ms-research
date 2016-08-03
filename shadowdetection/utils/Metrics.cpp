/**
  * Metrics designed to quantitatively evaluate results of shadow detection
  **/

#include "Metrics.h"

double calcDetectRate(Mat mask, Mat shadows) {
  // all concerning shadows
  // TP/TN - True Positive/Negative
  // FP/FN - False Positive/Negative

  int TPs = 0;
  int FNs = 0;

  assert(mask.rows == shadows.rows);
  assert(mask.cols == shadows.cols);

  for(int r = 0; r < mask.rows; r++) {
    for(int c = 0; c < mask.cols; c++) {
      int maskval = mask.at<unsigned char>(r, c);
      int shadval = shadows.at<unsigned char>(r, c);

      if(maskval == shadval) {
        if((int)shadval == 127) TPs++;
      } else if(maskval != shadval) {
        if((int)shadval == 127) FNs++;
      }
    }
  }
  return (double)TPs/(double)(FNs+TPs);
}

double calcDiscrimRate(Mat mask, Mat shadows) {
  // all concerning foreground
  // TP/TN - True Positive/Negative
  // FP/FN - False Positive/Negative

  int TPf = 0;
  int FNf = 0;

  assert(mask.rows == shadows.rows);
  assert(mask.cols == shadows.cols);

  for(int r = 0; r < mask.rows; r++) {
    for(int c = 0; c < mask.cols; c++) {
      int maskval = mask.at<unsigned char>(r, c);
      int shadval = shadows.at<unsigned char>(r, c);

      if(maskval == shadval) {
        if((int)shadval == 255) TPf++;
      } else if(maskval != shadval) {
        if((int)shadval == 255) FNf++;
      }
    }
  }
  return (double)TPf/(double)(FNf+TPf);
}
