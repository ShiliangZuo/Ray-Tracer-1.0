#pragma once

#include<cmath>
#include "Vector3D.h"
#include "Constants.h"

class Sphere : public GeoObject {
public:
	Vec3f center;
	float radius;
	float radius2;
	Material material;

	Sphere(
		Vec3f c,
		float r,
		Material mat
	) : center(c), radius(r), radius2(r*r),material(mat)
	{
	}

	bool intersect(Ray ray, float &dist, ShadeRec &rec) const {
		Vec3f l = center - ray.origin;
		float len = l.length();
		float cosTheta = ray.dir.dot(l.normalize());
		float delta = 4 * len*len*cosTheta*cosTheta - (4 * len * len - 4 * radius2);

		if (delta < 0) {
			return false;
		}
		float t0 = (len*cosTheta * 2 - sqrt(delta)) / 2;
		float t1 = (len*cosTheta * 2 + sqrt(delta)) / 2;
		if (t0 <= RAY_MIN_DIST)
			t0 = t1;
		if (t0 <= RAY_MIN_DIST)
			return false;
		dist = t0;
		rec.material = material;
		rec.hitPoint = ray.origin + ray.dir * dist;
		rec.normal = rec.hitPoint - center;
		rec.normal = rec.normal.normalize();
		return true;
	}
};
