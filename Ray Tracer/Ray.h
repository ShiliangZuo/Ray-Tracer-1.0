#pragma once

#include "Vector3D.h"

class Ray {
public:
	Vec3f origin;
	Vec3f dir;
	Ray(Vec3f o, Vec3f d) : origin(o), dir(d)
	{
	}
};
