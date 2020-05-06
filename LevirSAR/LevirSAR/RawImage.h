#ifndef RAWIMAGE_H
#define RAWIMAGE_H

#include "LevirSAR.h"

class RawImage
{
private:
	char *m_filename;
	int m_width;
	int m_height;

	char *m_bufferFile;

public:
    RawImage(char *filename, int width, int height);
    ~RawImage();

	// 以Mat格式返回RAW整个图像
	Mat returnWholeImg();
	// 给定图像块的左上点坐标和尺寸，以Mat格式返回RAW图像中的图像块
	Mat returnImgPatch(int x, int y, int width, int height);
};

#endif
