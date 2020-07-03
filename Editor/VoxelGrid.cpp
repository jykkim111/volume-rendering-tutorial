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
	vector<vec3> temp(width * height * depth);
	dataPos = temp;


	//give all data points a coordinate 
	for (double kk = 0; kk < depth; kk++) {
		for (double jj = 0; jj < height; jj++) {
			for (double ii = 0; ii < width; ii++) {
				int index = ii + jj * height + kk * height * width;
				dataPos[index] = vec3(ii, jj, kk);
			}
		}
	}

	cout << dataPos[0] << " " << dataPos[1000];
}

//retrieve index of voxel 
int VoxelGrid::getDataPosition(vec3 position) { 
	return position[0] + position[1] * height + position[2] * width * height;;
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
	if (coordinate[0] < width && coordinate[1] < height && coordinate[2] < depth && coordinate[0] >= 0 && coordinate[1] >= 0 && coordinate[2] >= 0) {
		return true;
	}

	return false;
}

float VoxelGrid::triInterp(vec3 samplePoint) {

	vec3 a000, b100, c010, d110, e001, f101, g011, h111;
	int valueA, valueB, valueC, valueD, valueE, valueF, valueG, valueH;

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
	valueA = data[getDataPosition(a000)];
	valueB = data[getDataPosition(b100)];
	valueC = data[getDataPosition(c010)];
	valueD = data[getDataPosition(d110)];
	valueE = data[getDataPosition(e001)];
	valueF = data[getDataPosition(f101)];
	valueG = data[getDataPosition(g011)];
	valueH = data[getDataPosition(h111)];

	float valueAB = linearInterp(samplePoint, valueA, valueB);
	float valueCD = linearInterp(samplePoint, valueC, valueD);
	float valueABCD = bilinearInterp(samplePoint, valueAB, valueCD);
	float valueEF = linearInterp(samplePoint, valueE, valueF);
	float valueGH = linearInterp(samplePoint, valueG, valueH);
	float valueEFGH = bilinearInterp(samplePoint, valueEF, valueGH);

	return (1 - samplePoint[2]) * valueABCD + samplePoint[2] * valueEFGH;

}


float VoxelGrid::linearInterp(vec3 samplePoint, float value1, float value2) {
	return (1 - samplePoint[0]) * value1 + samplePoint[0] * value2;
}

float VoxelGrid::bilinearInterp(vec3 samplePoint, float value1, float value2) {
	return (1 - samplePoint[1]) * value1 + samplePoint[1] * value2;

}

vec3 VoxelGrid::normalize(vec3 samplePoint) {
	vec3 normalizedPoint;

	float normalizedX = samplePoint[0] / width;
	float normalizedY = samplePoint[1] / height;
	float normalizedZ = samplePoint[2] / depth;
	normalizedPoint = vec3(width, height, depth);

	return normalizedPoint;
}




