#include "Vector3D.h"

class Camera {
public:
	Vec3f position;
	Vec3f direction;
	Vec3f up;
	Vec3f right;
	int height;
	int width;
	float fov;

public:
	Camera() {
		position = Vec3f(0);
		direction = Vec3f(0, 0, -1);
		up = Vec3f(0, 1, 0);
		right = direction.cross(up);
		width = 640;
		height = 480;
		fov = 50;
	}
	Camera(Vec3f pos, Vec3f dir, Vec3f u, int w, int h, float f) {
		position = pos;
		direction = dir;
		right = direction.cross(up);
		up = u;
		width = w;
		height = h;
		fov = f;
	}
};