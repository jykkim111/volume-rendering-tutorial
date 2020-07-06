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

bool Ray::intersect(Ray& r) {
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin = (bounds[r.sign[0]].x - r.getOrigin()[0]) * r.invdir.x;
    tmax = (bounds[1 - r.sign[0]].x - r.orig.x) * r.invdir.x;
    tymin = (bounds[r.sign[1]].y - r.orig.y) * r.invdir.y;
    tymax = (bounds[1 - r.sign[1]].y - r.orig.y) * r.invdir.y;

    if ((tmin > tymax) || (tymin > tmax))
        return false;
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    tzmin = (bounds[r.sign[2]].z - r.orig.z) * r.invdir.z;
    tzmax = (bounds[1 - r.sign[2]].z - r.orig.z) * r.invdir.z;

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;
}

