#pragma once

#include "Ray.h"
#include "Material.h"

class GeoObject {
public:
	virtual bool intersect(Ray ray, float &dist, ShadeRec &rec) const = 0;
};
