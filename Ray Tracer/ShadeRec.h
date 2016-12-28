#pragma once

#include "Vector3D.h"
#include "Material.h"

class ShadeRec {
public:
	Material material;
	Vec3f hitPoint;
	Vec3f normal;
};
