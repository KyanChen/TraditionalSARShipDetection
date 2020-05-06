#include "SeaLandSeg.h"

SeaLandSeg::SeaLandSeg(const char* loadpath)
{
	precision = 10000;
	erode_size = 4;
	load_shorelines(loadpath);
}

SeaLandSeg::~SeaLandSeg()
{
}

Mat SeaLandSeg::get_mask(double corner[4][2], int mask_height, int mask_width)
{
	Paths clip(1), solution;

	/*double corner[4][2];
	corner[0][0] = image_geo[0];
	corner[0][1] = image_geo[3];
	corner[1][0] = image_geo[0] + image_width * image_geo[1];
	corner[1][1] = image_geo[3] + image_width * image_geo[4];
	corner[2][0] = image_geo[0] + image_width * image_geo[1] + image_height * image_geo[2];
	corner[2][1] = image_geo[3] + image_width * image_geo[4] + image_height * image_geo[5];
	corner[3][0] = image_geo[0] + image_height * image_geo[2];
	corner[3][1] = image_geo[3] + image_height * image_geo[5];*/

	IntPoint pti;
	pti.X = corner[0][0] * precision; pti.Y = corner[0][1] * precision;
	clip[0].push_back(pti);
	pti.X = corner[1][0] * precision; pti.Y = corner[1][1] * precision;
	clip[0].push_back(pti);
	pti.X = corner[2][0] * precision; pti.Y = corner[2][1] * precision;
	clip[0].push_back(pti);
	pti.X = corner[3][0] * precision; pti.Y = corner[3][1] * precision;
	clip[0].push_back(pti);

	Clipper c;
	c.AddPaths(shorelines, ptSubject, true);
	c.AddPaths(clip, ptClip, true);
	c.Execute(ctIntersection, solution, pftNonZero, pftNonZero);

	Mat mask = draw_mask(solution, corner, mask_height, mask_width);
	erode(mask, mask, getStructuringElement(MORPH_RECT, Size(erode_size, erode_size)));

	return mask;
}

void SeaLandSeg::load_shorelines(const char* loadpath)
{
	/*
	FILE* fp = fopen(loadpath, "rb");

	IntPoint pti;
	Path curPolyLine;
	int poly_num;
	int point_num;
	float lon, lat;
	fread(&poly_num, sizeof(int), 1, fp);
	for (int i = 0; i < poly_num; i++)
	{
		fread(&point_num, sizeof(int), 1, fp);
		for (int j = 0; j < point_num; j++)
		{
			fread(&lon, sizeof(float), 1, fp);
			fread(&lat, sizeof(float), 1, fp);
			pti.X = lon * precision; pti.Y = lat * precision;
			curPolyLine.push_back(pti);
		}
		shorelines.push_back(curPolyLine);
		curPolyLine.clear();
	}

	fclose(fp);
	*/
}

Mat SeaLandSeg::draw_mask(Paths& solution, double corner[4][2], int mask_height, int mask_width)
{
	Mat mask(mask_height, mask_width, CV_8UC1);
	mask.setTo(0);

	Point2f src[4]; Point2f dst[4];
	src[0].x = corner[0][0]; src[0].y = corner[0][1];
	src[1].x = corner[1][0]; src[1].y = corner[1][1];
	src[2].x = corner[2][0]; src[2].y = corner[2][1];
	src[3].x = corner[3][0]; src[3].y = corner[3][1];

	dst[0].x = 0; dst[0].y = 0;
	dst[1].x = mask_width; dst[1].y = 0;
	dst[2].x = mask_width; dst[2].y = mask_height;
	dst[3].x = 0; dst[3].y = mask_height;

	Mat M = getAffineTransform(src, dst);

	double trueLon, trueLat;
	int x, y;
	int polynum = solution.size();
	Point** pts;
	pts = new Point * [polynum];
	int* nPts = new int[polynum];
	for (int i = 0; i < polynum; i++)
	{
		nPts[i] = solution[i].size();
		pts[i] = new Point[nPts[i]];
		for (int j = 0; j < nPts[i]; j++)
		{
			trueLon = (double)solution[i][j].X / precision;
			trueLat = (double)solution[i][j].Y / precision;

			x = M.at<double>(0, 0) * trueLon + M.at<double>(0, 1) * trueLat + M.at<double>(0, 2);
			y = M.at<double>(1, 0) * trueLon + M.at<double>(1, 1) * trueLat + M.at<double>(1, 2);

			pts[i][j] = Point(x, y);
		}
	}

	fillPoly(mask, (const Point**)pts, (const int*)nPts, polynum, Scalar(255), 8, 0, Point(0, 0));

	// ÊÍ·ÅÄÚ´æ
	delete[] nPts;

	for (int i = 0; i < polynum; i++)
		delete[] pts[i];

	delete[] pts;

	return mask;
}
