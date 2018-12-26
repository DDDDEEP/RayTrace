#pragma once
#include "bitmap_image.hpp"
#include <vector>
#include "Point.h"
using namespace std;

bool saveBmp(const vector<vector<ColorPoint>>& res, const string& outFilePath)
{
	int height = res.size();
	if (height <= 0) return false;
	int width = res[0].size();
	if (width <= 0) return false;
	bitmap_image image(height, width);
	for (int i = 0; i < image.height(); ++i)
	{
		for (int j = 0; j < image.width(); ++j)
		{
			rgb_t color;
			color.red = res[i][j].color.red*255;
			color.green = res[i][j].color.green*255;
			color.blue = res[i][j].color.blue*255;
			image.set_pixel(i, image.width() - 1 - j, color);
		}
	}
	image.save_image(outFilePath);
	return true;
}