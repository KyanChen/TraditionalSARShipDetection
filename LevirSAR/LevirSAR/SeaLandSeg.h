#include "LevirSAR.h"
#include "SeaLandSeg\clipper.hpp"

using namespace ClipperLib;

class SeaLandSeg
{
private:
	Paths shorelines;

	void load_shorelines(const char *loadpath);
	Mat draw_mask(Paths &solution, double corner[4][2], int mask_height, int mask_width);

public:
    SeaLandSeg(const char *loadpath);
    ~SeaLandSeg();

	Mat get_mask(double image_geo[6], int image_height, int image_width, int mask_height, int mask_width);
};