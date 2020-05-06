#ifndef SEALANDSEG_H
#define SEALANDSEG_H

#include "LevirSAR.h"
#include "SeaLandSeg\clipper.hpp"

using namespace ClipperLib;

class SeaLandSeg
{
private:
	int precision;
	int erode_size;
	Paths shorelines;

	void load_shorelines(const char* loadpath);
	Mat draw_mask(Paths& solution, double corner[4][2], int mask_height, int mask_width);

public:
	SeaLandSeg(const char* loadpath);
	~SeaLandSeg();

	Mat get_mask(double corner[4][2], int mask_height, int mask_width);
};

#endif
