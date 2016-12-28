#pragma once

#include "Vector3D.h"

enum LightType { POINT = 0, DIRECTIONAL = 1 };

class Light {
	// Haven't consider spotlight and area light, may be extended.
public:
	LightType type;
	Vec3f position;
	Vec3f color;
	Vec3f attenuation;	//How does the light decay with distance
	Vec3f lightDir;		// Direction to light, to reduce computation

	Light(LightType t, Vec3f p, Vec3f c, Vec3f at) : type(t), position(p), color(c), attenuation(at)
	{
	}
};
