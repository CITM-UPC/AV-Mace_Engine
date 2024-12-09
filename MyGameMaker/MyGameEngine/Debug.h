#pragma once

#include "types.h"
#include "BoundingBox.h"
#include "GameObject.h"
//#include "intersect.h"

void drawAxis(double size);
void drawWiredQuad(const vec3& v0, const vec3& v1, const vec3& v2, const vec3& v3);
void drawBoundingBox(const BoundingBox& bbox);
void drawFloorGrid(double size, double step);
void drawDebugInfoForGraphicObject(const GameObject& obj, bool good = false);
//void drawSegment(const Segment& s);
//void drawIntersectionPoint(const vec3& point);
