#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>

#include "Component.h"
#include "BoundingBox.h"
#include "Shader.h"
#include "Model.h"

class Mesh : public Component
{
	std::vector<glm::vec3 > _vertices;
	std::vector<unsigned int> _indices;

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

	BoundingBox _boundingBox;

public:
	Mesh(bool active, GameObject* owner) : Component(active, owner) {}
	~Mesh() {}

	const auto& vertices() const { return _vertices; }
	const auto& indices() const { return _indices; }
	const auto& boundingBox() const { return _boundingBox; }

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