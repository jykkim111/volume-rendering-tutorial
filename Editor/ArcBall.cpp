#include "ArcBall.h"
#include <math.h>


ArcBall :: ArcBall(float mouseWidth, float mouseHeight) {
	this->startVector = vec3();
	this->endVector = vec3();
	setBounds(mouseWidth, mouseHeight);
}

void ArcBall :: setBounds(float newWidth, float newHeight) {
	if (newWidth > 1.0f && newHeight > 1.0f) {
		mouseWidth = 1.0f / ((newWidth - 1.0f) * 0.5f);
		mouseHeight = 1.0f / ((newHeight - 1.0f) * 0.5f);
	}
}

void ArcBall::mapToSphere(vec2 screenPoint, vec3 &vector) {

	vec2 tempPoint = vec2(screenPoint[0], screenPoint[1]);

	//scale down to [-1...1]
	tempPoint[0] = tempPoint[0] * this->mouseWidth - 1.0f;
	tempPoint[1] = 1.0f - tempPoint[1] * this->mouseHeight;

	float r = tempPoint[0] * tempPoint[0] + tempPoint[1] * tempPoint[1];

	if (r > 1.0) {
		float norm = (float)(1.0 / sqrt(r));
		vector[0] = tempPoint[0] * norm;
		vector[1] = tempPoint[1] * norm;
		vector[2] = 0.0f;
	}
	else {
		vector[0] = tempPoint[0];
		vector[1] = tempPoint[1];
		vector[2] = (float)sqrt(1.0f - r);
	}
}

void ArcBall::clickPoint(vec2 clickPoint) {
	mapToSphere(clickPoint, this->startVector);
}

double ArcBall::getRotationAngle(vec3 startPoint, vec3 endPoint) {
	double dot = startPoint * endPoint;
	double length = sqrt(endPoint[0] * endPoint[0] + endPoint[1] * endPoint[1] + endPoint[2] * endPoint[2]) *
		sqrt(startPoint[0] * startPoint[0] + startPoint[1] * startPoint[1] + startPoint[2] * startPoint[2]);
	return acos(dot / length);
}


void ArcBall::dragToRotate(vec3 startPoint, vec3 endPoint) {
	//Map the end point to sphere
	


	
}


