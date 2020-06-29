#pragma once

#include <vector>
#include <sstream>
#include "algebra3.h"

using namespace std;

struct Voxel {
	vec3 color;
	double transmissivity;
};


class VoxelGrid
{
private:
	int width, height, depth;
	double voxelSize;
	vector<Voxel*> voxelGrid;


public:

	VoxelGrid();
	VoxelGrid(double size, int width, int height, int depth);



	int getVoxelIndex(int x, int y, int z); // Get array index of voxel given grid index
	vector<Voxel*> getVoxelGrid(); // Get voxel grid
	double getVoxelSize(); // Get voxel size
	int getWidth();
	int getHeight();
	int getDepth();
	bool isInsideGrid(vec3 coordinate);
};

