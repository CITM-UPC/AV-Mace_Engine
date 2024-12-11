#include "Ray.h"

// Constructor
Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
    : origin(origin), direction(glm::normalize(direction)) {}

// Get a point along the ray at a specified distance
glm::vec3 Ray::getPoint(float distance) const {
    return origin + direction * distance;
}