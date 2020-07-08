#include "AABBox.h"


AABBox::AABBox(const vec3 min, const vec3 max) : min(min), max(max) 
{
    bounds[0] = min;
    bounds[1] = max;
}


bool AABBox::intersect(const Ray& r, float& t) const {
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin = (bounds[r.sign[0]][0] - r.origin[0]) * r.inverseDir[0];
    tmax = (bounds[1-r.sign[0]][0] - r.origin[0]) * r.inverseDir[0];
    tymin = (bounds[r.sign[1]][1] - r.origin[1]) * r.inverseDir[1];
    tymax = (bounds[1-r.sign[1]][1] - r.origin[1]) * r.inverseDir[1];

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    tzmin = (bounds[r.sign[2]][2] - r.origin[2]) * r.inverseDir[2];
    tzmax = (bounds[r.sign[2]][2] - r.origin[2]) * r.inverseDir[2];

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    t = tmin;

    if (t < 0) {
        t = tmax;
        if (t < 0) return false;
    }

    return true;
}



