#pragma once

#include "algebra3.h"
#include <vector>
#include "Ray.h"

class AABBox
{
public:
    vec3 min;
    vec3 max;
    vec3 bounds[2];
    AABBox(const vec3 min, const vec3 max);
    bool intersect(const Ray& r, float& t) const;
};

