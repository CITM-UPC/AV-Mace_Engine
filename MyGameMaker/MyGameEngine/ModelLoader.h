#pragma once

#include <memory>
#include "Model.h"
#include "GameObject.h"

class ModelLoader
{
public:
	ModelLoader() = default;
	~ModelLoader() = default;

	void load(const std::string& filename, std::vector<std::shared_ptr<Model>>& models) const;
	void load(Shapes shape, std::shared_ptr<Model>& model);
	GameObject* loadFromFile(const std::string& filename);
	void saveFBXToCustomFormat(const std::string& fbxFilePath, const std::string& outputFilePath);
	GameObject* loadFromCustomFormat(const std::string& filePath);
};