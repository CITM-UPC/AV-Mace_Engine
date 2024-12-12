#pragma once

#include "types.h"

#include <array>

struct BoundingBox {
	vec3 min = vec3(0, 0, 0);
	vec3 max = vec3(0, 0, 0);

	vec3 center() const { return (min + max) * 0.5; }
	vec3 size() const { return max - min; }

	const vec3& v000() const { return min; }
	vec3 v001() const { return vec3(min.x, min.y, max.z); }
	vec3 v010() const { return vec3(min.x, max.y, min.z); }
	vec3 v011() const { return vec3(min.x, max.y, max.z); }
	vec3 v100() const { return vec3(max.x, min.y, min.z); }
	vec3 v101() const { return vec3(max.x, min.y, max.z); }
	vec3 v110() const { return vec3(max.x, max.y, min.z); }
	const vec3& v111() const { return max; }

	std::array<vec3, 8> vertices() const { return { v000(), v001(), v010(), v011(), v100(), v101(), v110(), v111() }; }

	BoundingBox() = default;
	BoundingBox(const vec3* vertices, size_t num_verts);

	BoundingBox operator+(const BoundingBox& other) const;

	bool isEmpty() const { return min == max; }

	BoundingBox(const vec3& min, const vec3& max) : min(min), max(max) {};

	// Genera un AABB transformado
	BoundingBox toAABB(const glm::mat4& transform) const {
		auto verts = vertices();
		vec3 new_min = vec3(std::numeric_limits<float>::max());
		vec3 new_max = vec3(std::numeric_limits<float>::lowest());

		for (const vec3& vert : verts) {
			vec3 transformed = vec3(transform * glm::vec4(vert, 1.0f)); // Transformar vértice
			new_min = glm::min(new_min, transformed); // Comparar mínimos
			new_max = glm::max(new_max, transformed); // Comparar máximos
		}

		return BoundingBox{ new_min, new_max };
	}
};


BoundingBox operator*(const mat4& mat, const BoundingBox& bbox);

