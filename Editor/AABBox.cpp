#include "AABBox.h"


AABBox::AABBox(const vec3 min, const vec3 max) : min(min), max(max) 
{

}


bool AABBox::intersect(const Ray& r, float& t) const {
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin = (min[0] - r.origin[0]) / r.direction[0];
    tmax = (max[0] - r.origin[0]) / r.direction[0];
    tymin = (min[1] - r.origin[1]) / r.direction[1];
    tymax = (max[1] - r.origin[1]) / r.direction[1];

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    tzmin = (min[2] - r.origin[2]) / r.direction[2];
    tzmax = (max[2] - r.origin[2]) / r.direction[2];

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



