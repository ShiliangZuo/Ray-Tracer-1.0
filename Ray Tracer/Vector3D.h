#pragma once
#include <iostream>

template<typename T>
class Vec3
{
public:
	T x, y, z;
	Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
	Vec3(T xx) : x(xx), y(xx), z(xx) {}
	Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
	//Vec3(Vec3 vec) : x(vec.x), y(vec.y), z(vec.z) {}
	Vec3 normalize()
	{
		T nor2 = length2();
		Vec3 normalized;
		if (nor2 > 0) {
			T invNor = 1 / sqrt(nor2);
			normalized.x = x*invNor;
			normalized.y = y*invNor;
			normalized.z = z*invNor;
		}
		return normalized;
	}
	Vec3 &clamp()
	{
		if (x > 1)
			x = 1;
		if (y > 1)
			y = 1;
		if (z > 1)
			z = 1;
		return *this;
	}
	Vec3<T> operator * (const T &f) const { return Vec3<T>(x * f, y * f, z * f); }
	Vec3<T> operator * (const Vec3<T> &v) const { return Vec3<T>(x * v.x, y * v.y, z * v.z); }
	T dot(const Vec3<T> &v) const { return x * v.x + y * v.y + z * v.z; }
	Vec3<T> operator - (const Vec3<T> &v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }
	Vec3<T> operator + (const Vec3<T> &v) const { return Vec3<T>(x + v.x, y + v.y, z + v.z); }
	bool operator == (const Vec3<T> &v) const { return x == v.x && y == v.y && z == v.z; }
	bool operator != (const Vec3<T> &v) const { return x != v.x || y != v.y || z != v.z; }
	Vec3<T>& operator += (const Vec3<T> &v) { x += v.x, y += v.y, z += v.z; return *this; }
	Vec3<T>& operator *= (const Vec3<T> &v) { x *= v.x, y *= v.y, z *= v.z; return *this; }
	Vec3<T> operator - () const { return Vec3<T>(-x, -y, -z); }
	Vec3<T> cross(const Vec3<T> &v) const { return Vec3<T>(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); }
	T length2() const { return x * x + y * y + z * z; }
	T length() const { return sqrt(length2()); }
	friend std::ostream & operator << (std::ostream &os, const Vec3<T> &v)
	{
		os << "[" << v.x << " " << v.y << " " << v.z << "]";
		return os;
	}
};

typedef Vec3<float> Vec3f;
typedef Vec3<float> Point;