#pragma once

#include "Constants.h"
#include "Vector3D.h"
#include <cmath>

class Triangle : public GeoObject {
public:
	Point v1, v2, v3;
	Material material;
	Vec3f normal;
	float D;
	Triangle(Point v, Point vv, Point vvv, Material mat) : v1(v), v2(vv), v3(vvv), material(mat)
	{
		Vec3f vec21 = Vec3f(vv - v);
		Vec3f vec31 = Vec3f(vvv - v);
		normal = vec21.cross(vec31);
		D = -(normal.x * v1.x + normal.y * v1.y + normal.z * v1.z);
	}

	bool intersect(Ray ray, float &dist, ShadeRec &rec) const {
		//printf("Called tri intersect\n");
		float t = -(normal.dot(ray.origin) + D) / normal.dot(ray.dir);
		if (t <= RAY_MIN_DIST)
			return false;
		Point inter = Point(ray.origin + ray.dir*t);
		Vec3f i1 = v1 - inter;
		Vec3f i2 = v2 - inter;
		Vec3f i3 = v3 - inter;
		Vec3f c12 = i1.cross(i2);
		Vec3f c23 = i2.cross(i3);
		Vec3f c31 = i3.cross(i1);
		if (c12.dot(c23) < 0)
			return false;
		if (c23.dot(c31) < 0)
			return false;
		if (c31.dot(c12) < 0)
			return false;

		dist = t;
		rec.material = material;
		rec.normal = normal;
		rec.normal = rec.normal.normalize();
		rec.hitPoint = ray.origin + ray.dir*dist;
		return true;
	}
};
