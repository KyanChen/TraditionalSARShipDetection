#include "LevirSAR.h"

class FineDetection
{
private:
    HOGDescriptor *m_hog;
	double *m_w;
#ifndef GET_DATA
    float get_hog_score(Mat& img);
#endif // !GET_DATA
	
#ifdef GET_DATA
    void FineDetection::writeTxt(string name, vector<float> bboxes, vector<float> descriptors);
    vector<float> FineDetection::get_hog_score(Mat& img);
#endif // GET_DATA

public:
    FineDetection();
    ~FineDetection();

    void run(Mat &img, vector<Rect> &targets, vector<Rect> &final_targets);
#ifdef GET_DATA
    void run(string filename, Mat& img, vector<Rect>& targets, vector<Rect>& final_targets);
#endif // GET_DATA

    int FineDetection::get_weights(string name, double* weights);
};
