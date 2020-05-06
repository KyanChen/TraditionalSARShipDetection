#ifndef COARSEDETECTION_H
#define COARSEDETECTION_H

#include "LevirSAR.h"

class CoarseDetection
{
private:
	// 滑动窗口大小
	int target_size;
	int guard_size;
	int background_size;
	//形态学滤波模板大小
	int close_size;
	int open_size;
	// 候选框筛选相关参数
	int min_width;
	int min_height;
	int max_width;
	int max_height;
	float min_ratio;
	float max_ratio;
	int min_area;
	int max_area;
	// 滤波模板
	Mat background_kernel;
	// 高斯模板（FPGA）
	double* gaussian_template;
	// 回形模板（FPGA）
	double* squarehole_template;

	// 生成滤波模板
	Mat get_background_kernel();
	// 得到候选区域图
	Mat get_candi_map(Mat& img);
	// 生成高斯模板（FPGA）
	void get_gaussian_template_fpga(double* gaussian_template, int kernel_size, double sigma = 0);
	// 生成回形模板（FPGA）
	void get_squarehole_template_fpga(double* squarehole_template, int background_size, int guard_size);
	// 滤波（FPGA）
	void filter_fpga(uchar* img_src, uchar* img_dst, int img_row, int img_col, int kernel_size, double* filter_template);
	// 得到候选区域图（FPGA）
	Mat get_candi_map_fpga(Mat& img);
	// 连通域分析
	void connect_search(Mat& candiMap, vector<Rect>& targets);

public:
	CoarseDetection();
	~CoarseDetection();

	void run(Mat& img, vector<Rect>& targets);
};

#endif
