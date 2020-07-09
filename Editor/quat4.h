#pragma once
#include <math.h>

class quat4 {

public:
	float x, y, z, w;

	quat4() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
	}

	quat4(double angle, float x, float y, float z) {
		double s = sqrt(x * x + y * y + z * z);
		double c = 1.0 / s;

		x *= c;
		y *= c;
		z *= c;

		float omega = (float)(-0.5f * angle);
		s = (float)sin(omega);

		this->x = (float)(s * x);
		this->y = (float)(s * y);
		this->z = (float)(s * z);
		this->w = (float)cos(omega);


	}

};