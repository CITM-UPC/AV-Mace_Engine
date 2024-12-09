#pragma once
#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include <glm/glm.hpp>
#include <string>

#include "Component.h"
#include "BufferObject.h"
#include "BoundingBox.h"

#include "Shader.h"

#include "Model.h"

class Mesh : public Component
{
	std::vector<glm::vec3 > _vertices;
	std::vector<unsigned int> _indices;

	BufferObject _vertices_buffer;
	BufferObject _indices_buffer;
	BufferObject _texCoords_buffer;
	BufferObject _normals_buffer;
	BufferObject _colors_buffer;

	BoundingBox _boundingBox;

	//la nostra manera de carregar un model
	std::shared_ptr<Model> model;
	std::string filePath;

	//for debbuging
	std::vector<glm::vec3> normalLines;
	unsigned int vBNormalsLinesID = -1;
	bool debugNormals = false;
	std::unique_ptr<Shader> m_NormalLinesShader;

	std::vector<glm::vec3> faceNormalLines;
	unsigned int vBfaceNormalLinesID = -1;
	bool debugFaceNormals = false;

public:
	explicit Mesh(GameObject* owner) : Component(owner) {}
	Mesh(GameObject* owner, const glm::vec3* verts, size_t num_verts, const unsigned int* indexs, size_t num_indexs) : Component(owner) { load(verts, num_verts, indexs, num_indexs); }

	const auto& vertices() const { return _vertices; }
	const auto& indices() const { return _indices; }
	const auto& boundingBox() const { return _boundingBox; }

	void draw() const;
	void load(const glm::vec3* verts, size_t num_verts, const unsigned int* indexs, size_t num_indexs);
	void loadTexCoords(const glm::vec2* texCoords);
	void loadNormals(const glm::vec3* normals);
	void loadColors(const glm::u8vec3* colors);

	//la nostra manera de carregar un model
	void loadToOpenGL();
	void drawModel() const;
	void setModel(std::shared_ptr<Model> model) { this->model = model; }
	std::string& getFilePath() { return filePath; }
	void setFilePath(const std::string& filePath) { this->filePath = filePath; }

	//for debbuging
	void loadNormalsToOpenGL();
	void drawNormals() const;
	void setDebugNormals(bool& debugNormals) { this->debugNormals = debugNormals; }
	bool getDebugNormals() { return debugNormals; }

	void loadFaceNormalsToOpenGL();
	void drawFaceNormals() const;
	void setDebugFaceNormals(bool& debugFaceNormals) { this->debugFaceNormals = debugFaceNormals; }
	bool getDebugFaceNormals() { return debugFaceNormals; }
};

#endif // !__MESH_H__