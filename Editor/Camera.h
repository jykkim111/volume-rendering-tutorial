#pragma once
#include <vector>
#include "algebra3.h"


class Camera
{
private:
	vec3 position;

public: 

	Camera(vec3 position);

	vec3 getPosition();
	void lookAt(vec3 direction);
};

