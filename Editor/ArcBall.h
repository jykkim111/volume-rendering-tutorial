#pragma once
#include <GL\glew.h>
#include "algebra3.h"
#include "quat4.h"

class ArcBall
{
private: 
	vec3 startVector;
	vec3 endVector;
	float mouseWidth;
	float mouseHeight;
public:
	ArcBall(float mouseWidth, float mouseHeight);
	void setBounds(float newWidth, float newHeight);
	void mapToSphere(vec2 point, vec3 &vector);
	void clickPoint(vec2 clickPoint);
	double getRotationAngle(vec3 startPoint, vec3 endPoint);
	void dragToRotate(vec3 startPoint, vec3 endPoint);

};

