#include "FineDetection.h"

FineDetection::FineDetection(string filename)
{
	this->m_hog = new HOGDescriptor(Size(64, 64), Size(16, 16), Size(8, 8), Size(8, 8), 9);
	this->m_w = new double[1766];
	get_weights(filename, this->m_w);
}

FineDetection::~FineDetection()
{
	delete this->m_hog;
	delete[] this->m_w;
}

void FineDetection::run(Mat& img, vector<Rect>& targets, vector<Rect>& final_targets)
{
	for (int i = 0; i < targets.size(); i++)
	{
		// 扩充切片大小
		int x1 = targets[i].x - targets[i].width / 2;
		int y1 = targets[i].y - targets[i].height / 2;
		int x2 = targets[i].x + 3 * targets[i].width / 2;
		int y2 = targets[i].y + 3 * targets[i].height / 2;

		if (x1 < 0) x1 = 0; if (x2 >= img.cols) x2 = img.cols - 1;
		if (y1 < 0) y1 = 0; if (y2 >= img.rows) y2 = img.rows - 1;

		Mat patch = img(Range(y1, y2), Range(x1, x2));

		// HOG+SVM分类器
		float confiScore = get_hog_score(patch);
		//cout << confiScore << endl;

		if (confiScore > 0.)
		{
			final_targets.push_back(targets[i]);
		}
	}
}

float FineDetection::get_hog_score(Mat& img)
{
	// 切片统一到相同大小
	Mat hogImg = img.clone();
	resize(hogImg, hogImg, Size(64, 64));

	// 提取HOG特征
	vector<float> descriptors;
	m_hog->compute(hogImg, descriptors, Size(1, 1), Size(0, 0));

	// SVM
	float confiScore = 0.;
	int d_id;
	float scale = m_w[1765];
	for (d_id = 0; d_id < descriptors.size(); ++d_id)
	{
		confiScore += (float)m_w[d_id] * descriptors[d_id] / scale;
	}
	confiScore += (float)m_w[d_id];

	return confiScore;
}

int FineDetection::get_weights(string filename, double* weights)
{
	ifstream srcFile(filename, ios::in); //以文本模式打开in.txt备读
	if (!srcFile) { //打开失败
		cout << "error opening source file." << endl;
		return -1;
	}
	double temp_weight;
	int total = 0;
	while (srcFile >> temp_weight) {
		//可以像用cin那样用ifstream对象
		//cout << temp_weight << endl;
		weights[total++] = temp_weight;
	}
	return 0;
}



void FineDetection::get_features(string filename, Mat& img, vector<Rect>& targets, vector<Rect>& final_targets)
{
	for (int i = 0; i < targets.size(); i++)
	{
		// 扩充切片大小
		int x1 = targets[i].x - targets[i].width / 2;
		int y1 = targets[i].y - targets[i].height / 2;
		int x2 = targets[i].x + 3 * targets[i].width / 2;
		int y2 = targets[i].y + 3 * targets[i].height / 2;

		if (x1 < 0) x1 = 0; if (x2 >= img.cols) x2 = img.cols - 1;
		if (y1 < 0) y1 = 0; if (y2 >= img.rows) y2 = img.rows - 1;

		Mat patch = img(Range(y1, y2), Range(x1, x2));

		string img_format = ".jpg";
		//提取目录和文件名
		int pos = filename.rfind("\\");
		string dir = filename.substr(0, pos);
		string name = filename.substr(pos, filename.length());
		string img_path = dir.substr(0, dir.rfind("\\")) + "\\patch_img" + name.substr(0, name.rfind(img_format)) + "_" + to_string(i) + ".jpg";
		string txt_path = dir.substr(0, dir.rfind("\\")) + "\\patch_txt" + name.substr(0, name.rfind(img_format)) + ".txt";
		imwrite(img_path, patch);

		// 切片统一到相同大小
		Mat hogImg = patch.clone();
		resize(hogImg, hogImg, Size(64, 64));
		// 提取HOG特征
		vector<float> descriptors;
		m_hog->compute(hogImg, descriptors, Size(1, 1), Size(0, 0));
		vector<float> bboxes = { float(targets[i].x), float(targets[i].y), float(targets[i].x + targets[i].width), float(targets[i].y + targets[i].height) };
		
		ofstream ofs;
		ofs.open(txt_path, ios::app);
		char s[20];
		for (int i = 0; i < bboxes.size(); i++) {
			// sprintf_s(s, "%.2f", text.at(i));
			ofs << bboxes.at(i) << " ";
		}
		for (int i = 0; i < descriptors.size(); i++) {
			// sprintf_s(s, "%.2f", text.at(i));
			ofs << descriptors.at(i);
			if (i != descriptors.size() - 1) {
				ofs << " ";
			}
		}
		ofs << endl;
		ofs.close();
	}
}