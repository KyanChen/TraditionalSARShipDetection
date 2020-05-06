#include "LevirSAR.h"
#include "RawImage.h"
#include "SeaLandSeg.h"
#include "CoarseDetection.h"
#include "GetFeatures.h"

#ifndef GET_DATA
int main(int argc, char** argv)
{
	// 相关配置文件信息
	char* shoreline_path = "./shorelines.b";
	string svm_weight_path = "./matlabweights_w_b_scale.txt";

	// 图像路径
	char* image_path = "./GF3_SAY_QPSI_002349_E119.8_N40.0_20170119_L1A_HV_L10002133574.raw";

	// 图像大小信息
	int image_height = 8296;
	int image_width = 7669;

	// 图像地理信息
	double corner[4][2];
	corner[0][0] = 120.030687;
	corner[0][1] = 40.151945;
	corner[1][0] = 119.588319;
	corner[1][1] = 40.219710;
	corner[2][0] = 119.481937;
	corner[2][1] = 39.811611;
	corner[3][0] = 119.921502;
	corner[3][1] = 39.743920;

	// 图像分块信息
	int block_height = 512;
	int block_width = 512;

	// 海陆分割
#ifdef TIME_DEBUG
	clock_t starttime = clock();
#endif // TIME_DEBUG

	SeaLandSeg SLS(shoreline_path);
	Mat mask = SLS.get_mask(corner, ceil((float)image_height / block_height), ceil((float)image_width / block_width));

#ifdef TIME_DEBUG
	clock_t endtime = clock();
	cout << "SeaLandSeg:" << (endtime - starttime) / 1000. << " s" << endl;
#endif // TIME_DEBUG

	// 精检测和粗检测初始化
	vector<Rect> proposalTargets;
	CoarseDetection CD;
	vector<Rect> finalTargets;
	FineDetection FD(svm_weight_path);

	// 分块读图，粗检测和精检测
	int actual_height, actual_width;
	Mat actual_block;
	Mat input_block(block_height, block_width, CV_8UC1, Scalar(0));
	Mat whole_image(image_height, image_width, CV_8UC1, Scalar(0));
	RawImage RI(image_path, image_width, image_height);
	for (int y = 0; y < image_height; y = y + block_height)
	{
		actual_height = (y + block_height >= image_height) ? image_height - y : block_height;
		for (int x = 0; x < image_width; x = x + block_width)
		{
			actual_width = (x + block_width >= image_width) ? image_width - x : block_width;
			if (mask.at<uchar>(y / block_height, x / block_width) == 255) continue;
			actual_block = RI.returnImgPatch(x, y, actual_width, actual_height);
			actual_block.copyTo(input_block.colRange(0, actual_width).rowRange(0, actual_height));

			CD.run(input_block, proposalTargets);
			FD.run(input_block, proposalTargets, finalTargets);
			for (int i = 0; i < finalTargets.size(); i++)
			{
				rectangle(input_block, finalTargets[i], Scalar(255), 2);
			}

			input_block.colRange(0, actual_width).rowRange(0, actual_height).copyTo(whole_image.colRange(x, x + actual_width).rowRange(y, y + actual_height));

			proposalTargets.clear();
			finalTargets.clear();
			input_block.setTo(0);
		}
	}

#ifdef TIME_DEBUG
	endtime = clock();
	cout << "TraditionalSAR:" << (endtime - starttime) / 1000. << " s" << endl;
#endif // TIME_DEBUG
	imwrite("result_t.jpg", whole_image);

	return 0;
}
#endif // !GET_DATA

