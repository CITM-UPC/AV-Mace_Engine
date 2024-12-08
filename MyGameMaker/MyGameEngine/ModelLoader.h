#ifndef MYGAMEENGINE_MODELLOADER_H
#define MYGAMEENGINE_MODELLOADER_H
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
	std::shared_ptr<GameObject> loadFromFile(const std::string& filename);
	void saveFBXToCustomFormat(const std::string& fbxFilePath, const std::string& outputFilePath);
	std::shared_ptr<GameObject> loadFromCustomFormat(const std::string& filePath);
};

#endif // MYGAMEENGINE_MODELLOADER_H