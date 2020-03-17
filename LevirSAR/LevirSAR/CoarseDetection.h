#ifndef COARSEDETECTION_H
#define COARSEDETECTION_H

#include "LevirSAR.h"

class CoarseDetection
{
private:
    // 高斯滤波模板大小
	int kernel_size1;
	int kernel_size2;
	//形态学滤波模板大小
    int close_size;
    int open_size;
    // 候选框筛选相关参数
    int min_width;
	int min_height;
    float min_ratio;
    float max_ratio;
	int min_area;

	// 得到候选区域图
	Mat get_candi_map(Mat &img);
	// 连通域分析
    void connect_search(Mat &candiMap, vector<Rect> &targets);

public:
    CoarseDetection();
    ~CoarseDetection();

    void run(Mat &img, vector<Rect> &targets);
};

#endif
