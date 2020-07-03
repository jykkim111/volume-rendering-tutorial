#include "Camera.h"

Camera::Camera(vec3 position)
	:
	position(position)
{


}

vec3 Camera :: getPosition() {
	return this->position;
}


/*
void Camera::lookAt(vec3 direction) {
	this->position = direction;
}
*/
