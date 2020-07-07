#include "VoxelGrid.h"
#include <math.h>

using namespace std;


VoxelGrid::VoxelGrid(int width, int height, int depth, vector<short> data)
	:
	width(width),
	height(height),
	depth(depth),
	data(data)
{
	
}

//retrieve index of voxel 
int VoxelGrid::getDataIndex(vec3 position) { 
	return position[0] + position[1] * height + (position[2] - 10) * width * height;;
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
	if (coordinate[0] < width && coordinate[1] < height && coordinate[2] + 10 < depth && coordinate[0] >= 0 && coordinate[1] >= 0 && coordinate[2] >= 0) {
		return true;
	}

	return false;
}

float VoxelGrid::triInterp(vec3 samplePoint) {

	vec3 a000, b100, c010, d110, e001, f101, g011, h111;
	float valueA, valueB, valueC, valueD, valueE, valueF, valueG, valueH;

	//Get the position of vertices of voxel
	a000 = vec3(floor(samplePoint[0]),floor(samplePoint[1]), floor(samplePoint[2]));
	b100 = vec3(ceil(samplePoint[0]), floor(samplePoint[1]), floor(samplePoint[2]));
	c010 = vec3(floor(samplePoint[0]), ceil(samplePoint[1]), floor(samplePoint[2]));
	d110 = vec3(ceil(samplePoint[0]), ceil(samplePoint[1]), floor(samplePoint[2]));

	e001 = vec3(floor(samplePoint[0]), floor(samplePoint[1]), ceil(samplePoint[2]));
	f101 = vec3(ceil(samplePoint[0]), floor(samplePoint[1]), ceil(samplePoint[2]));
	g011 = vec3(floor(samplePoint[0]), ceil(samplePoint[1]), ceil(samplePoint[2]));
	h111 = vec3(ceil(samplePoint[0]), ceil(samplePoint[1]), ceil(samplePoint[2]));


	//Get the corresponding data value of the vertices
	valueA = data[getDataIndex(a000)];
	valueB = data[getDataIndex(b100)];
	valueC = data[getDataIndex(c010)];
	valueD = data[getDataIndex(d110)];
	valueE = data[getDataIndex(e001)];
	valueF = data[getDataIndex(f101)];
	valueG = data[getDataIndex(g011)];
	valueH = data[getDataIndex(h111)];

	float valueAB = linearInterp(samplePoint, valueA, valueB);
	float valueCD = linearInterp(samplePoint, valueC, valueD);
	float valueABCD = bilinearInterp(samplePoint, valueAB, valueCD);
	float valueEF = linearInterp(samplePoint, valueE, valueF);
	float valueGH = linearInterp(samplePoint, valueG, valueH);
	float valueEFGH = bilinearInterp(samplePoint, valueEF, valueGH);


	if (ceil(samplePoint[2]) - samplePoint[2] == 0) {
		return valueEFGH;
	}
	else if (samplePoint[2] - floor(samplePoint[2]) == 0) {
		return valueABCD;
	}
	else {
		return (ceil(samplePoint[2]) - samplePoint[2]) * valueABCD + (samplePoint[2] - floor(samplePoint[2])) * valueEFGH;
	}

}


float VoxelGrid::linearInterp(vec3 samplePoint, float value1, float value2) {
	if (ceil(samplePoint[0]) - samplePoint[0] == 0) {
		return value2;
	}
	else if (samplePoint[0] - floor(samplePoint[0]) == 0) {
		return value1;
	}
	else {
		return (ceil(samplePoint[0]) - samplePoint[0]) * value1 + (samplePoint[0] - floor(samplePoint[0])) * value2;
	}
}

float VoxelGrid::bilinearInterp(vec3 samplePoint, float value1, float value2) {
	if (ceil(samplePoint[1]) - samplePoint[1] == 0) {
		return samplePoint[1] * value2;
	}
	else if (samplePoint[1] - floor(samplePoint[1]) == 0) {
		return (samplePoint[1] + 1) * value1;
	}
	else {
		return (ceil(samplePoint[1]) - samplePoint[1]) * value1 + (samplePoint[1] - floor(samplePoint[1])) * value2;
	}
}

vec3 VoxelGrid::normalize(vec3 samplePoint) {
	vec3 normalizedPoint;
	float normalizedX = samplePoint[0] / width;
	float normalizedY = samplePoint[1] / height;
	float normalizedZ = samplePoint[2] / depth;
	normalizedPoint = vec3(width, height, depth);

	return normalizedPoint;
}





