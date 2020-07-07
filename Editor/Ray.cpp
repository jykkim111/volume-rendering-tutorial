#include "Ray.h"

Ray::Ray(const vec3 origin, const vec3 direction)
    :
    origin(origin),
    direction(direction)
{
    inverseDir[0] = 1.0 / direction[0];
    inverseDir[1] = 1.0 / direction[1];
    inverseDir[2] = 1.0 / direction[2];

    sign[0] = (inverseDir[0] < 0);
    sign[1] = (inverseDir[1] < 0);
    sign[2] = (inverseDir[2] < 0);
}

vec3 Ray::getOrigin() {
	return this->origin;
}

vec3 Ray::getDirection() {
	return this->direction;
}

vec3 Ray::getCurrentPos(float t) {
	return getOrigin() + (t * getDirection());
}



