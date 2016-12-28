#include <cstdlib> 
#include <cstdio> 
#include <cmath> 
#include <fstream> 
#include <vector> 
#include <iostream> 
#include <cassert> 
#include <algorithm>
#include <sstream>

#include "Ray.h"
#include "Light.h"
#include "GeoObject.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Scene.h"
#include "SimpleObject.h"

//Right Hand Coordinate System

//Took ambient light, eta for granted, TODO, need to change this
//Took Camera position, center for granted, TOOD, may need to change this


int main(int args, char* argv[]) {
	printf("Beginning...\n");

	printf("Received %d Arguments: \n", args);
	for (int i = 0; i < args; ++i) {
		std::cout << argv[i] << std::endl;
	}

	SimpleOBJ::CSimpleObject inputObj;
	inputObj.LoadFromObj(argv[1]);

	Scene scene;

	std::vector<GeoObject*>& objects = scene.objects;;
	std::vector<Light>& lights = scene.lights;

	Vec3f color = Vec3f(0.5, 0.2, 0.3);
	Vec3f zero(0);
	Vec3f one(1);
	Material purple;
	purple = Material(color, color, color, color, zero, 10);

	for (int i = 0; i < inputObj.m_nTriangles; ++i) {
		Vec3f pos[3];
		for (int index = 0; index < 3; ++index) {
			int k = inputObj.m_pTriangleList[i][index];
			pos[index] = inputObj.m_pVertexList[k];
		}
		Triangle *tri;
		tri = new Triangle(
			pos[0],
			pos[2],
			pos[1],
			purple
		);
		objects.push_back(tri);
	}

	//objects.push_back(new Sphere(Vec3f(0.5, 1, -10), 2, purple));

	Vec3f white = Vec3f(1.0);
	Vec3f black = Vec3f(0.0);
	Material mat[2];
	mat[1] = Material(white, white, white, white, white, 1);
	mat[0] = Material(black, black, black, black, black, 1);

	for (int xx = -4; xx <= 3; xx++)
		for (int zz = -4; zz >= -11; zz--) {
			int flag;
			if ((xx + zz) & 1)
				flag = 1;
			else
				flag = 0;
			Triangle *tri;
			tri = new Triangle(
				Vec3f(xx, -1, zz),
				Vec3f(xx + 1, -1, zz),
				Vec3f(xx + 1, -1, zz - 1),
				mat[flag]
			);
			objects.push_back(tri);
			tri = new Triangle(
				Vec3f(xx, -1, zz),
				Vec3f(xx + 1, -1, zz - 1),
				Vec3f(xx, -1, zz - 1),
				mat[flag]
			);
			objects.push_back(tri);
		}
	for (int xx = -4; xx <= 3; ++xx) {
		for (int yy = -1; yy <= 6; ++yy) {
			int flag;
			if ((xx + yy) & 1)
				flag = 0;
			else
				flag = 1;
			Triangle *tri;
			tri = new Triangle(
				Vec3f(xx, yy, -12),
				Vec3f(xx + 1, yy, -12),
				Vec3f(xx + 1, yy + 1, -12),
				mat[flag]
			);
			objects.push_back(tri);
			tri = new Triangle(
				Vec3f(xx, yy, -12),
				Vec3f(xx + 1, yy + 1, -12),
				Vec3f(xx, yy + 1, -12),
				mat[flag]
			);
			objects.push_back(tri);
		}
	}

	Vec3f lpos = Vec3f(0, 8, -8);
	Vec3f lcolor = Vec3f(1);
	Light light = Light(POINT, lpos, lcolor, Vec3f(0));
	lights.push_back(light);

	printf("%d Objects\n", (int)objects.size());
	scene.camera = Camera();
	scene.camera.position = Vec3f(0, 2, 5);
	scene.Render("./Output/Cube.ppm");

	printf("Done!!!! Finished Ray Tracing %d Objects!!!!!\n", (int)objects.size());
	system("pause");
	return 0;
}

/*Vec3f trace(
	const Ray ray,
	const std::vector<GeoObject*> &objects,
	const std::vector<Light> &lights,
	int depth)

{
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
				rec.hitPoint = rectmp.hitPoint;
				rec.material = rectmp.material;
				rec.normal = rectmp.normal;
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
			ShadeRec rectmp;
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
			Vec3f reflectDir = - (rec.normal * (rec.normal.dot(raydir)) * 2 - raydir);
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
		Vec3f raydirection = -(rec.normal * cosTheta * 2 - ray.dir);
		Ray reflectRay = Ray(reflectOrigin, raydirection);
		reflectColor = trace(reflectRay, objects, lights, depth + 1) * rec.material.specular;
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
		refractColor = trace(Ray(refractOrigin, refractDir), objects, lights, depth + 1) * rec.material.transparent;
	}
	Vec3f ret = specular + diffuse + ambient + reflectColor + refractColor;
	return ret.clamp();
}

void render(const std::vector<GeoObject*> &objects, const std::vector<Light> &lights)
{
	unsigned width = 640, height = 480;
	Vec3f *image = new Vec3f[width * height], *pixel = image;
	float invWidth = 1 / float(width), invHeight = 1 / float(height);
	float fov = 50, aspectratio = width / float(height);
	float angle = tan(M_PI * 0.5 * fov / 180.);

	// Trace rays
	for (unsigned y = 0; y < height; ++y) {
		for (unsigned x = 0; x < width; ++x, ++pixel) {
			float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio;
			float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
			Vec3f origin(0, 0, 0);
			Vec3f dir(xx, yy, -1);
			dir = dir.normalize();
			Ray ray = Ray(origin, dir);
			*pixel = trace(ray, objects, lights, 0);
		}
		printf("%d * %d\n", y, width);
	}

	// Save result to a PPM image (keep these flags if you compile under Windows)
	std::ofstream ofs("./RefractpReflect.ppm", std::ios::out | std::ios::binary);
	ofs << "P6\n" << width << " " << height << "\n255\n";
	for (unsigned i = 0; i < width * height; ++i) {
		ofs << (unsigned char)(std::min(float(1), image[i].x) * 255) <<
			(unsigned char)(std::min(float(1), image[i].y) * 255) <<
			(unsigned char)(std::min(float(1), image[i].z) * 255);
	}
	ofs.close();
	delete[] image;
}*/


/*bool ReadValues(std::stringstream & s, const int num, float * values)
{
	for (int i = 0; i < num; i++)
	{
		s >> values[i];
		if (s.fail())
		{
			std::cerr << "Failed Reading Values " << i << " Will Skip.\n";
			return false;
		}
	}
	return true;
}

void parseOBJ(std::vector<GeoObject*> &objects) {
	std::vector<Point> points;
	std::ifstream file("bunny.obj");
	std::string str, cmd;
	float values[3];
	Vec3f v(1, 1, 1);
	Vec3f zero(0);
	Material material = Material(v, v, v, zero, zero, 2);
	while (std::getline(file, str))
	{
		if (str[0] == '#') {
			continue;
		}

		std::stringstream s(str);
		s >> cmd;
		if (cmd == "v") {
			ReadValues(s, 3, values);
			points.push_back(Point(values[0] * 100, values[1] * 100, values[2] * 100));
		}
		if (cmd == "f") {
			ReadValues(s, 3, values);
			//printf("%d\n", (int)objects.size());
			Triangle *tri = new Triangle(
				points[(int)values[0]-1],
				points[(int)values[1]-1],
				points[(int)values[2]-1],
				material
			);
			objects.push_back(tri);
		}
	}
}
*/