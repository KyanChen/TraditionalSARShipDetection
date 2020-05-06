#include "RawImage.h"

RawImage::RawImage(char *filename, int width, int height)
{
	if (filename == nullptr || width <= 0 || height <= 0)
		throw std::runtime_error("Input params error!");

	std::ifstream dataFile(filename, std::ios::binary);
	if (!dataFile.is_open())
		throw std::runtime_error("Failed to open this raw image!");
	else
		std::cout << "Read raw image successfully." << std::endl;

	m_filename = filename;
	m_width = width;
	m_height = height;

	std::filebuf *pbuf;

	// 获取filestr对应buffer对象的指针 
	pbuf = dataFile.rdbuf();

	// 调用buffer对象方法获取文件大小
	int fileLength = pbuf->pubseekoff(0, std::ios::end, std::ios::in);
	if (fileLength != m_height*m_width)
		throw std::runtime_error("The given height and width cannot match the length of the raw image.");
	pbuf->pubseekpos(0, std::ios::in);

	// 分配内存空间
	m_bufferFile = new char[fileLength]; // 析构时释放掉

	// 获取文件内容
	pbuf->sgetn(m_bufferFile, fileLength);
	dataFile.close();

}

RawImage::~RawImage()
{
	if (m_bufferFile != nullptr)
		delete[] m_bufferFile;
}


/*
函数：returnWholeImg()
作用：以Mat格式返回RAW整个图像
*/
Mat RawImage::returnWholeImg()
{
	Mat image_result = Mat(m_height, m_width, CV_8UC1);
	uchar *image_result_ptr = image_result.ptr<uchar>(0);
	for (int i = 0; i < m_height; i++){
		for (int j = 0; j < m_width; j++){
			image_result_ptr[i * m_width + j] = m_bufferFile[i*m_width + j];
		}
	}

	return image_result;

}



/*
函数：returnImgPatch()
作用：给定图像块的左上点坐标和尺寸，以Mat格式返回RAW图像中的图像块
参数：
@x：左上点坐标中水平方向位置
@y: 左上点坐标中竖直方向位置
@width： 图像块的宽度
@height: 图像块的高度
*/
Mat RawImage::returnImgPatch(int x, int y, int width, int height)
{
	if (x<0 || y<0 || (x+width-1) > m_width || (y+height-1) > m_height)
		throw std::runtime_error("Input params error!");

	Mat image_result = Mat(height, width, CV_8UC1);
	uchar *image_result_ptr = image_result.ptr<uchar>(0);
	for (int i = y; i < y + height; i++){
		for (int j = x; j < x + width; j++){
			image_result_ptr[(i - y) * width + j - x] = m_bufferFile[i*m_width + j];
		}
	}

	return image_result;
}