#pragma once
#include <vector>
#include "algebra3.h"

using namespace std;


class Ray
{
public:
	Ray(const vec3 origin, const vec3 direction);
	vec3 origin;
	vec3 direction;
	vec3 inverseDir;
	vec3 bounds[2];
	int sign[3];
	vec3 getOrigin();
	vec3 getDirection();
	vec3 getCurrentPos(float t);
};

