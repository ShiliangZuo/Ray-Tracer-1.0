#pragma once
#include "Vector3D.h"

class Material {
public:
	Vec3f ambient;//area light
	Vec3f diffuse;//diffusion reflection
	Vec3f specular;//mirror reflection
	Vec3f emission;
	Vec3f transparent;//refraction
	float shininess;
	Material()
	{
		Vec3f v(0);
		ambient = v; diffuse = v; specular = v; emission = v; transparent = v; shininess = 0;
	}
	Material(const Vec3f amb, const Vec3f dif, const Vec3f spec, 
		const Vec3f trans, const Vec3f emmi, float s)
	{
		ambient = amb;
		diffuse = dif;
		specular = spec;
		emission = emmi;
		transparent = trans;
		shininess = s;
	}
};