#include "VoxelGrid.h"
#include <math.h>

using namespace std;


VoxelGrid::VoxelGrid(int width, int height, int depth, vector<short> data) :
	width(width),
	height(height),
	depth(depth),
	data(data)
{
	
}

//retrieve index of datapoint
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
	if (coordinate[0] <= width - 1 && coordinate[1] <= height-1 && coordinate[2] <= depth + 9 && coordinate[0] >= 0 && coordinate[1] >= 0 && coordinate[2] >= 0) {
		return true;
	}

	return false;
}

double VoxelGrid::triInterp(vec3 samplePoint) {

	vec3 d000, c100, a010, b110, h001, g101, e011, f111;
	double valueA, valueB, valueC, valueD, valueE, valueF, valueG, valueH;

	//Get the position of vertices of voxel
	d000 = vec3(floor(samplePoint[0]),floor(samplePoint[1]), floor(samplePoint[2]));
	c100 = vec3(ceil(samplePoint[0]), floor(samplePoint[1]), floor(samplePoint[2]));
	a010 = vec3(floor(samplePoint[0]), ceil(samplePoint[1]), floor(samplePoint[2]));
	b110 = vec3(ceil(samplePoint[0]), ceil(samplePoint[1]), floor(samplePoint[2]));

	h001 = vec3(floor(samplePoint[0]), floor(samplePoint[1]), ceil(samplePoint[2]));
	g101 = vec3(ceil(samplePoint[0]), floor(samplePoint[1]), ceil(samplePoint[2]));
	e011 = vec3(floor(samplePoint[0]), ceil(samplePoint[1]), ceil(samplePoint[2]));
	f111 = vec3(ceil(samplePoint[0]), ceil(samplePoint[1]), ceil(samplePoint[2]));


	//Get the corresponding data value of the vertices
	valueD = data[getDataIndex(d000)];
	valueC = data[getDataIndex(c100)];
	valueA = data[getDataIndex(a010)];
	valueB = data[getDataIndex(b110)];
	valueH = data[getDataIndex(h001)];
	valueG = data[getDataIndex(g101)];
	valueE = data[getDataIndex(e011)];
	valueF = data[getDataIndex(f111)];

	//cout << valueD << "\n" << valueC << "\n" << valueA << "\n" << valueB << "\n" << valueH << "\n" << valueG << "\n" << valueE << "\n" << valueF << "\n" << "end\n";

	double valueAB = linearInterp(samplePoint, valueA, valueB);
	double valueCD = linearInterp(samplePoint, valueC, valueD);
	double valueABCD = bilinearInterp(samplePoint, valueAB, valueCD);
	double valueEF = linearInterp(samplePoint, valueE, valueF);
	double valueGH = linearInterp(samplePoint, valueG, valueH);
	double valueEFGH = bilinearInterp(samplePoint, valueEF, valueGH);


	double z = samplePoint[2] - floor(samplePoint[2]);
	return (1 - z) * valueABCD + z * valueEFGH;
}


double VoxelGrid::linearInterp(vec3 samplePoint, float valueA, float valueB) {
	double x = samplePoint[0] - floor(samplePoint[0]);
	return (1 - x) * valueA + x * valueB;
}

double VoxelGrid::bilinearInterp(vec3 samplePoint, float valueA, float valueB) {
	double y = samplePoint[1] - floor(samplePoint[1]);
	return y * valueA + (1 - y) * valueB;
}

vec3 VoxelGrid::normalize(vec3 samplePoint) {
	vec3 normalizedPoint;
	float normalizedX = samplePoint[0] / width;
	float normalizedY = samplePoint[1] / height;
	float normalizedZ = samplePoint[2] / depth;
	normalizedPoint = vec3(width, height, depth);

	return normalizedPoint;
}





