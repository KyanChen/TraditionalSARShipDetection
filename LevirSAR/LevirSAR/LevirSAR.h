#ifndef LEVIRSAR_H
#define LEVIRSAR_H

#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;



//#define DEBUG
#define GET_DATA

#ifdef GET_DATA
int getProposal(string filename);
#endif

#endif
