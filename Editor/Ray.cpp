#include "Ray.h"

Ray::Ray(const vec3 origin, const vec3 direction) {
	this->origin = origin;
	this->direction = direction;
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

