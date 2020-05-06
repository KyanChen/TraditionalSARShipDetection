#include "LevirSAR.h"

class FineDetection
{
private:
    HOGDescriptor* m_hog;
    double* m_w;

    float get_hog_score(Mat& img);
    int get_weights(string filename, double* weights);

public:
    FineDetection(string filename);
    ~FineDetection();

    void run(Mat& img, vector<Rect>& targets, vector<Rect>& final_targets);


    void get_features(string filename, Mat& img, vector<Rect>& targets, vector<Rect>& final_targets);
};