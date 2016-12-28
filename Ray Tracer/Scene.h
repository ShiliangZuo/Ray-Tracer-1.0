#pragma once

#include "GeoObject.h"
#include "Light.h"
#include "Ray.h"
#include "Camera.h"
#include "ShadeRec.h"
#include <vector>


class Scene {
public:
	void Render(char* savePath);
	Vec3f Trace(const Ray& ray, int depth);
public:
	std::vector<GeoObject*> objects;
	std::vector<Light> lights;
	Camera camera;
};