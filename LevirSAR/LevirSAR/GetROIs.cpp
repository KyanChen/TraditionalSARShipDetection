#include "GetROIs.h"
#include "LevirSAR.h"
#include <io.h>
string img_format = ".jpg";

void getFiles(string path, vector<string>& files, string mode)
{
	//文件句柄    
	long long hFile = 0;
	//文件信息    
	struct _finddata_t fileinfo;
	string p = path + "\\" + "*" + mode;
	if ((hFile = _findfirst(p.c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之    
			//如果不是,加入列表    
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.append("\\").append(fileinfo.name), files, mode);
			}
			else
			{
				files.push_back(path + "\\" + fileinfo.name);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
int get_weights(string filename, double* weights) {
	ifstream srcFile(filename, ios::in); //以文本模式打开in.txt备读
	if (!srcFile) { //打开失败
		cout << "error opening source file." << endl;
		return -1;
	}
	double temp_weight;
	int total = 0;
	while (srcFile >> temp_weight) {
		//可以像用cin那样用ifstream对象
		weights[total++] = temp_weight;
	}
	return 0;
}


void main() {

	vector<string> files;
	string imgPath = "F:\\DataSet\\sar_ship\\SSDD数据以及标签\\JPEGImages";
	

	// 得到候选区坐标及特征向量到TXT文本中
	getFiles(imgPath, files, img_format);
	for(int i = 0; i < files.size(); i++) {
		getProposal(files.at(i));
	}
	/*
	double w[3];
	get_weights(imgPath, w);
	cout << w;
	*/
}