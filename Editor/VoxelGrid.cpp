#include "VoxelGrid.h"

using namespace std;

VoxelGrid::VoxelGrid()
{
	voxelSize = 0.0;
	width = 0;
	height = 0;
	depth = 0;
}

VoxelGrid::VoxelGrid(double size, int width, int height, int depth)
{
	voxelSize = size;
	width = width;
	height = height;
	depth = depth;

	// Create voxel grid
	for (double kk = 0; kk < depth; kk++) {
		for (double jj = 0; jj < height; jj++) {
			for (double ii = 0; ii < width; ii++) {
				Voxel* voxel = new Voxel();
				voxelGrid.push_back(voxel);
			}
		}
	}
}

int VoxelGrid::getVoxelIndex(int x, int y, int z) {
	return x + y * width + z * width * height;
}

vector<Voxel*> VoxelGrid::getVoxelGrid() { 
	return voxelGrid; 
}

double VoxelGrid::getVoxelSize() { 
	return voxelSize; 
}

int VoxelGrid::getWidth() { 
	return width; 
}

int VoxelGrid::getHeight() { 
	return height; 
}

int VoxelGrid::getDepth() { 
	return depth; 
}

bool VoxelGrid::isInsideGrid(vec3 coordinate)
{
	double x_max = width * voxelSize;
	double y_max = height * voxelSize;
	double z_max = depth * voxelSize;

	if (coordinate[0] < x_max && coordinate[1] < y_max && coordinate[2] < z_max && coordinate[0] >= 0 && coordinate[1] >= 0 && coordinate[2] >= 0) {
		return true;
	}

	return false;
}


