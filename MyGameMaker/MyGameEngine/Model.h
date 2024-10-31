#pragma once

#include <vector>
#include <string>

#include "types.h"

enum class Shapes
{
	CUBE,
	PLANE,
	MESH
};

struct ModelData
{
	unsigned int vBPosID = -1, vBTCoordsID = -1, iBID = -1, vA = -1;
	std::vector<vec3> vertexData;
	std::vector<unsigned int> indexData;
	std::vector<vec2> vertex_texCoords;
	std::vector<vec3> vertex_normals;
	std::vector<vec3> vertex_colors;
};

class Model
{
public:
	Model() {}
	~Model() {}

	std::string& GetMeshName() { return meshName; }

	ModelData& GetModelData() { return modelData; }

	void SetMeshName(const std::string& meshName) { this->meshName = meshName; }

	void SetModelData(const ModelData& modelData) { this->modelData = modelData; }
private:
	std::string meshName;
	ModelData modelData;
};
