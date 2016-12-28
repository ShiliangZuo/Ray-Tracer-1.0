#include "Scene.h"
#include "Constants.h"
#include <cstdlib> 
#include <cstdio> 
#include <fstream>
#include <algorithm>

Vec3f Scene::Trace(const Ray& ray, int depth) {
	if (depth == MAX_RAY_DEPTH)
		return Vec3f(0);

	const GeoObject* hitObject = NULL;
	float tNear = INFINITY;
	ShadeRec rec;
	ShadeRec rectmp;
	for (unsigned i = 0; i < objects.size(); ++i) {
		float t0 = 0;
		if (objects[i]->intersect(ray, t0, rectmp) == true) {
			//printf("Hit!");
			if (t0 < tNear) {
				tNear = t0;
				rec = rectmp;
				hitObject = objects[i];
			}
		}
	}
	if (hitObject == NULL)
		return Vec3f(0);

	Vec3f ambient = rec.material.ambient * ENVIRONMENT_LIGHT;
	//cast a ray from every light source, Local Lighting
	Vec3f diffuse = Vec3f(0);
	Vec3f specular = Vec3f(0);
	for (unsigned i = 0; i < lights.size(); ++i) {
		Light light = lights[i];
		Vec3f raydir = rec.hitPoint - light.position;
		float len = raydir.length();
		raydir = raydir.normalize();

		if (raydir.dot(rec.normal) > 0)
			continue;

		Point rayorigin = light.position;
		Ray ray = Ray(rayorigin, raydir);
		bool blocked = false;
		for (unsigned i = 0; i < objects.size(); ++i) {
			float t0 = 0;
			if (objects[i]->intersect(ray, t0, rectmp)) {
				if (t0 < len - HIT_POINT_BIAS) {
					blocked = true;
					break;
				}
			}
		}
		if (!blocked) {
			//Calculate diffuse
			float cosTheta = raydir.dot(rec.normal);
			if (cosTheta < 0) {
				diffuse += -rec.material.diffuse * light.color * cosTheta;
			}
			//Calculate specular
			Vec3f reflectDir = -(rec.normal * (rec.normal.dot(raydir)) * 2 - raydir);
			cosTheta = reflectDir.dot(rec.hitPoint.normalize());
			if (cosTheta > 0) {
				specular += rec.material.specular * light.color * pow(cosTheta, rec.material.shininess);
			}
		}
	}

	Vec3f reflectColor = Vec3f(0);
	Vec3f refractColor = Vec3f(0);
	// recursively compute EITHER reflection OR refraction
	if (rec.material.specular != Vec3f(0)) {
		Vec3f reflectOrigin = rec.hitPoint;
		float cosTheta = rec.normal.dot(ray.dir);
		Vec3f reflectDir = -(rec.normal * cosTheta * 2 - ray.dir);
		reflectColor = Trace(Ray(reflectOrigin, reflectDir), depth + 1) * rec.material.specular;
	}

	if (rec.material.transparent != Vec3f(0)) {
		Vec3f refractOrigin = rec.hitPoint;
		bool inside = false;
		if (ray.dir.dot(rec.normal) > 0) { //inside the object
			inside = true;
		}
		else {
			rec.normal = -rec.normal;
		}
		float eta = (!inside) ? CONST_ETA : 1 / CONST_ETA;
		float cos1 = rec.normal.dot(ray.dir);
		float k = 1 - (1 - cos1 * cos1) / (eta*eta);
		if (k < 0)
			k = 0;
		float cos2 = sqrt(k);
		Vec3f refractDir = ray.dir * (1 / eta) + rec.normal * (cos2 - cos1 / eta);
		refractDir = refractDir.normalize();
		refractColor = Trace(Ray(refractOrigin, refractDir), depth + 1) * rec.material.transparent;
	}
	Vec3f ret = specular + diffuse + ambient + reflectColor + refractColor;
	return ret.clamp();
}

void Scene::Render(char* savePath) {
	unsigned width = camera.width, height = camera.height;
	Vec3f *image = new Vec3f[width * height], *pixel = image;
	float invWidth = 1 / float(width), invHeight = 1 / float(height);
	float fov = camera.fov, aspectratio = width / float(height);
	float angle = tan(M_PI * 0.5 * fov / 180.);

	// Trace rays
	for (unsigned y = 0; y < height; ++y) {
		for (unsigned x = 0; x < width; ++x, ++pixel) {
			Vec3f direction = Vec3f(0);
			float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio;
			float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
			direction += camera.right * xx;
			direction += camera.up * yy;
			direction += camera.direction;

			Vec3f origin = camera.position;
			//Vec3f dir(xx, yy, -1);
			//dir = dir.normalize();
			direction = direction.normalize();
			Ray ray = Ray(origin, direction);
			*pixel = Trace(ray, 0);
		}
		printf("%d * %d\n", y, width);
	}

	// Save result to a PPM image (keep these flags if you compile under Windows)
	std::ofstream ofs(savePath, std::ios::out | std::ios::binary);
	ofs << "P6\n" << width << " " << height << "\n255\n";
	for (unsigned i = 0; i < width * height; ++i) {
		ofs << (unsigned char)(std::min(float(1), image[i].x) * 255) <<
			(unsigned char)(std::min(float(1), image[i].y) * 255) <<
			(unsigned char)(std::min(float(1), image[i].z) * 255);
	}
	ofs.close();
	delete[] image;
}