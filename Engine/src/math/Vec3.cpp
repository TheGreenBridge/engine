#include "Vec3.h"
#include <cmath>

Vec3 &Vec3::operator+=(const Vec3 &other) {
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

// NO USE!!!!!! 
Vec3 &Vec3::operator+=(const float value) {
	x += value;
	y += value;
	z += value;

	return *this;
}

Vec3 &Vec3::operator/=(const float value) {
	x /= value;
	y /= value;
	z /= value;

	return *this;
}

Vec3 &Vec3::operator-=(const Vec3 &other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;

	return *this;
}

Vec3 Vec3::operator*(const F32 value) const {
	Vec3 result;
	result.x = x * value;
	result.y = y * value;
	result.z = z * value;
	return result;
}

Vec3 Vec3::operator*(const Vec3 & other) const
{
	Vec3 result;
	result.x = x * other.x;
	result.y = y * other.y;
	result.z = z * other.z;
	return result;
}

Vec3 Vec3::operator+(const Vec3 &other) const {
	Vec3 result;
	result.x = x + other.x;
	result.y = y + other.y;
	result.z = z + other.z;

	return result;

}

Vec3 Vec3::operator-(const Vec3 &other) const {
	Vec3 result;
	result.x = x - other.x;
	result.y = y - other.y;
	result.z = z - other.z;

	return result;
}

F32 Vec3::dotProduct(const Vec3 &vector1, const Vec3 &vector2) {
	F32 result;
	result = vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
	return result;
}

F32 Vec3::distance(const Vec3& first, const Vec3& second)
{
	Vec3 distance = first - second;
	return distance.length();
}

Vec3 Vec3::crossProduct(const Vec3 &vector1, const Vec3 &vector2)
{
	Vec3 result;

	result.x = vector1.y * vector2.z - vector1.z * vector2.y;
	result.y = vector1.z * vector2.x - vector1.x * vector2.z;
	result.z = vector1.x * vector2.y - vector1.y * vector2.x;

	return result;
}

F32 Vec3::length() const {
	return sqrt(x*x + y*y + z*z);
}

void Vec3::normalize() {
	if (length() != 0)
		*this /= length();
}

Vec3 Vec3::normalize(Vec3 &vector) {
	Vec3 result = vector;
	F32 test = vector.length();
	result /= vector.length();

	return result;
}