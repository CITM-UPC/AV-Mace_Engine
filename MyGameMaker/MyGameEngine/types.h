#pragma once

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define BUFFER_SIZE 4096

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using mat4 = glm::dmat4;
using vec4 = glm::dvec4;
using vec3 = glm::dvec3;
using vec2 = glm::dvec2;
using quat = glm::quat;

namespace Colors {
	const glm::u8vec3 Red( 255, 0, 0);
	const glm::u8vec3 Green( 0, 255, 0);
	const glm::u8vec3 Blue( 0, 0, 255);
};


// ALGEBRA STRUCTS ----------------------------------------------------------------
struct Point { 
	vec3 p;
	Point(const vec3& p) : p(p) {}
};
struct Vector { 
	vec3 v; 
	Vector(const vec3& v) : v(v) {}
};
struct Plane { 
	Point origin; 
	Vector normal; 
	Plane(const Point& origin, const Vector& normal) : origin(origin), normal(normal) {}
};

inline Point operator*(const mat4& mat, const Point& point) {
	vec4 v4 = mat * vec4(point.p, 1.0);
	return Point(vec3(v4) / v4.w);
}
inline Vector operator*(const mat4& mat, const Vector& vec) { 
	return Vector(vec3(mat * vec4(vec.v, 0.0)));
}
inline Plane operator*(const mat4& mat, const Plane& plane) {
	return Plane(mat * plane.origin, mat * plane.normal);
}

// TYPEDEFS ----------------------------------------------------------------------
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;