#pragma once

#include <cmath>
#include "Vec3.h"
#include "math_def.h"

struct Transform;

struct mat4 {
	// 64 Bytes
	union {
		float vecs[4][4];
		float elements[4 * 4];
	};
	

	mat4();
	explicit mat4(float diagonal);
	explicit mat4(float x1, float x2, float x3, float x4, 
		float y1, float y2, float y3, float y4, 
		float z1, float z2, float z3, float z4, 
		float w1, float w2, float w3, float w4);
	
	mat4 & operator *=(const mat4 &other);
	mat4 operator * (const mat4 &other) const;
	float & operator[](const int index);

	mat4 & multiply(const mat4 &other);
	mat4 multiply(const mat4 &other) const;

	void transpose();
	void inverse();
	
	static mat4 Perspective(float fov, float aspectRatio, float near, float far);
	static mat4 Orthographic(const float &bottom, const float &top, const float &left, const float &right,
		const float &near, const float &far);
	static mat4 Translate(const Vec3 &translation);
	static mat4 Rotate(float angle, const Vec3& axis);

	static mat4 TransformToMat4(Transform& transform);
};