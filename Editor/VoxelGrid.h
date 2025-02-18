#pragma once

#include <vector>
#include <sstream>
#include "algebra3.h"

using namespace std;

struct Voxel {
	vec3 color;
	double transmissivity;
	int value;
};


class VoxelGrid
{
private:
	int width, height, depth;
	vector<short> data;


public:

	VoxelGrid(int width, int height, int depth, vector<short> data);
	

	int getDataIndex(vec3 position);
	int getWidth();
	int getHeight();
	int getDepth();
	bool isInsideGrid(vec3 coordinate);
	double triInterp(vec3 samplePoint);
	double linearInterp(vec3 samplePoint, float value1, float value2);
	double bilinearInterp(vec3 samplePoint, float value1, float value2);
	vec3 normalize(vec3 samplePoint);

};

