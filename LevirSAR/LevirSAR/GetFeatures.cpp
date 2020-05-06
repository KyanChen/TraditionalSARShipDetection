#include "GetFeatures.h"
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

int getProposal(string filename)
{
	Mat img = imread(filename, IMREAD_ANYDEPTH);

	img.convertTo(img, CV_8UC1);
	// imwrite(filename.substr(0, filename.length() - 4) + ".jpg", img);

	clock_t starttime = clock();
	vector<Rect> proposalTargets;
	CoarseDetection CD;
	CD.run(img, proposalTargets);
	clock_t endtime = clock();
	cout << "coarse:" << (endtime - starttime) / 1000. << " s" << endl;

	starttime = clock();
	vector<Rect> finalTargets;
	FineDetection FD("path");
	FD.get_features(filename, img, proposalTargets, finalTargets);
	endtime = clock();
	cout << "fine:" << (endtime - starttime) / 1000. << " s" << endl;
	return 0;
}

#ifdef GET_DATA
void main() {

	vector<string> files;
	string imgPath = "F:\\DataSet\\sar_ship\\SAR-Levir-Selected\\JPEGImages";


	// 得到候选区坐标及特征向量到TXT文本中
	getFiles(imgPath, files, img_format);
	for (int i = 0; i < files.size(); i++) {
		getProposal(files.at(i));
	}
}
#endif // GET_DATA