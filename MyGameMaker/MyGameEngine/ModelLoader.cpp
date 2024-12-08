#include "ModelLoader.h"

#include <assimp/cimport.h>    
#include <assimp/postprocess.h>
#include <assimp/scene.h>         
#include <assimp/mesh.h>
#include <filesystem>
#include <memory>
#include <fstream>
#include <functional>


#include "Texture.h"
#include "Material.h"
#include "Transform.h"
#include "Mesh.h"
#include "Scene.h"
#include "Engine.h"
#include "BoundingBox.h"

#include "Log.h"

void ModelLoader::load(const std::string& filename, std::vector<std::shared_ptr<Model>>& models) const
{
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	const aiScene* scene = aiImportFile(filename.c_str(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_GenUVCoords | 
		aiProcess_TransformUVCoords | 
		aiProcess_FlipUVs);

	// If the import failed, report it
	if (nullptr == scene) {
		LOG(LogType::LOG_WARNING, "Couldn't find %s", filename.c_str());
	}
	else {
		models.resize(scene->mNumMeshes);

		std::vector<std::shared_ptr<ModelData>> modelsData;
		modelsData.resize(scene->mNumMeshes);

		for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[i];
			modelsData[i] = std::make_shared<ModelData>();
			models[i] = std::make_shared<Model>();
			models[i]->SetMeshName(mesh->mName.C_Str());

			for (unsigned int j = 0; j < mesh->mNumVertices; j++) {

				// Coordenadas de los vértices
				aiVector3D vertex = mesh->mVertices[j];
				vec3 aux = vec3(vertex.x, vertex.y, vertex.z);
				modelsData[i]->vertexData.push_back(aux);

				// Coordenadas UV (si existen)
				if (mesh->mTextureCoords[0]) {  // Comprueba si hay UVs
					aiVector3D uv = mesh->mTextureCoords[0][j];
					aux.x = uv.x;  // Solo X y Y
					aux.y = uv.y;
				}
				modelsData[i]->vertex_texCoords.push_back(aux);

				if (mesh->HasNormals()) {  // Verifica si hay normales
					aiVector3D normal = mesh->mNormals[j];
					vec3 auxNormal(normal.x, normal.y, normal.z);
					modelsData[i]->vertex_normals.push_back(auxNormal);
				}

				if (mesh->HasVertexColors(0)) {  // Verifica si hay colores
					aiColor4D color = mesh->mColors[0][j];
					vec3 auxColor(color.r, color.g, color.b);
					modelsData[i]->vertex_colors.push_back(auxColor);
				}

			}

			for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
				aiFace face = mesh->mFaces[j];
				for (unsigned int k = 0; k < face.mNumIndices; k++) {
					modelsData[i]->indexData.push_back(face.mIndices[k]);
				}
			}

			models[i]->SetModelData(*modelsData[i]);

		}

	}
}

void ModelLoader::load(Shapes shape, std::shared_ptr<Model>& model)
{

	std::shared_ptr<ModelData> modelData = std::make_shared<ModelData>();
	model = std::make_shared<Model>();

	const float PI = 3.14159265358979323846f;
	const int slices = 36; // number of radial slices
	const int stacks = 18; // number of stacks (height divisions)
	const float radius = 1.0f; // radius of the sphere
	const float height = 2.0f; // height of the cylinder

	switch (shape)
	{
	case Shapes::CUBE:
		model->SetMeshName("Cube");

		modelData->vertexData = {
			vec3(-1,-1,-1),
			vec3(1,-1,-1), 
			vec3(1,1,-1),  
			vec3(-1,1,-1), 
			vec3(-1,-1,1), 
			vec3(1,-1,1),  
			vec3(1,1,1),   
			vec3(-1,1,1)  
		};

		modelData->vertex_colors = {
			vec3(0.8,0,0),
			vec3(0.0,1,0),
			vec3(0.0,0,1),
			vec3(1,1,1),
			vec3(1,0,1),
			vec3(0.5,1,0),
			vec3(0.8,1,0.5),
			vec3(0.8,0.3,0.6)
		};

		modelData->indexData = {
			0, 1, 2, 0, 2, 3,
			1, 5, 6, 1, 6, 2,
			5, 4, 7, 5, 7, 6,
			4, 0, 3, 4, 3, 7,
			3, 2, 6, 3, 6, 7,
			4, 5, 1, 4, 1, 0
		};
		break;
	
	case Shapes::PLANE:
		model->SetMeshName("Plane");

		modelData->vertexData = {
			vec3(-1,0,-1),
			vec3(1,0,-1),
			vec3(1,0,1),
			vec3(-1,0,1)
		};

		modelData->vertex_colors = {
			vec3(0.8,0,0),
			vec3(0.0,1,0),
			vec3(0.0,0,1),
			vec3(1,1,1)
		};

		modelData->indexData = {
			0, 1, 2, 0, 2, 3
		};
		break;
	
	case Shapes::SPHERE:
		model->SetMeshName("Sphere");

		for (int i = 0; i <= stacks; ++i)
		{
			float theta = i * PI / stacks; // latitude angle
			for (int j = 0; j <= slices; ++j)
			{
				float phi = j * 2.0f * PI / slices; // longitude angle
				float x = radius * sin(theta) * cos(phi);
				float y = radius * cos(theta);
				float z = radius * sin(theta) * sin(phi);

				modelData->vertexData.push_back(vec3(x, y, z));
				modelData->vertex_colors.push_back(vec3(1.0f, 0.0f, 0.0f)); // red color
			}
		}

		// Generate indices for the sphere
		for (int i = 0; i < stacks; ++i)
		{
			for (int j = 0; j < slices; ++j)
			{
				int first = i * (slices + 1) + j;
				int second = first + slices + 1;
				if (i != 0)
				{
					modelData->indexData.push_back(first);
					modelData->indexData.push_back(second);
					modelData->indexData.push_back(first + 1);
				}
				if (i != (stacks - 1))
				{
					modelData->indexData.push_back(second);
					modelData->indexData.push_back(second + 1);
					modelData->indexData.push_back(first + 1);
				}
			}
		}
		break;

	case Shapes::CYLINDER:
		model->SetMeshName("Cylinder");

		for (int i = 0; i <= stacks; ++i)
		{
			float y = (i / (float)stacks) * height - height / 2.0f;
			for (int j = 0; j < slices; ++j)
			{
				float theta = j * 2.0f * PI / slices;
				float x = radius * cos(theta);
				float z = radius * sin(theta);
				modelData->vertexData.push_back(vec3(x, y, z));
				modelData->vertex_colors.push_back(vec3(0.0f, 1.0f, 0.0f)); // green color
			}
		}

		// Create top and bottom caps of the cylinder
		for (int j = 0; j < slices; ++j)
		{
			float theta = j * 2.0f * PI / slices;
			float x = radius * cos(theta);
			float z = radius * sin(theta);
			modelData->vertexData.push_back(vec3(x, height / 2.0f, z)); // top cap
			modelData->vertexData.push_back(vec3(x, -height / 2.0f, z)); // bottom cap
			modelData->vertex_colors.push_back(vec3(0.0f, 1.0f, 0.0f)); // green color
		}

		// Create the cylinder body indices
		for (int i = 0; i < stacks; ++i)
		{
			for (int j = 0; j < slices; ++j)
			{
				int first = i * slices + j;
				int second = (i + 1) * slices + j;
				int next = (j + 1) % slices;
				int third = (i + 1) * slices + next;
				modelData->indexData.push_back(first);
				modelData->indexData.push_back(second);
				modelData->indexData.push_back(third);
				modelData->indexData.push_back(first);
				modelData->indexData.push_back(third);
				modelData->indexData.push_back(next);
			}
		}

		// Add top and bottom cap indices
		for (int j = 0; j < slices; ++j)
		{
			int top_center = modelData->vertexData.size() - 2 * slices;
			int bottom_center = modelData->vertexData.size() - slices;
			int first = j;
			int second = (j + 1) % slices;
			modelData->indexData.push_back(top_center);
			modelData->indexData.push_back(first);
			modelData->indexData.push_back(second);
			modelData->indexData.push_back(bottom_center);
			modelData->indexData.push_back(second + slices);
			modelData->indexData.push_back(first + slices);
		}
		break;

	case Shapes::CONE:
		model->SetMeshName("Cone");

		// Vertices for cone
		modelData->vertexData.push_back(vec3(0.0f, height / 2.0f, 0.0f)); // top (apex)
		modelData->vertex_colors.push_back(vec3(1.0f, 0.0f, 0.0f)); // red color

		for (int i = 0; i < slices; ++i)
		{
			float theta = i * 2.0f * PI / slices;
			float x = radius * cos(theta);
			float z = radius * sin(theta);
			modelData->vertexData.push_back(vec3(x, -height / 2.0f, z)); // base vertices
			modelData->vertex_colors.push_back(vec3(0.0f, 1.0f, 0.0f)); // green color
		}

		// Cone body indices
		for (int i = 0; i < slices; ++i)
		{
			int first = i + 1;
			int second = (i + 1) % slices + 1;
			modelData->indexData.push_back(0); // apex
			modelData->indexData.push_back(first);
			modelData->indexData.push_back(second);
		}

		// Base indices (not required but could be included for completeness)
		for (int i = 0; i < slices; ++i)
		{
			int first = i + 1;
			int second = (i + 1) % slices + 1;
			modelData->indexData.push_back(first);
			modelData->indexData.push_back(second);
			modelData->indexData.push_back(slices + 1);
		}
		break;

	case Shapes::TORUS:
		model->SetMeshName("Torus");

		// Torus parameters
		float R = 1.0f;  // major radius (distance from the center of the torus to the center of the tube)
		float r = 0.4f;  // minor radius (radius of the tube itself)
		int numTorusSlices = 36; // Number of slices around the tube
		int numTorusStacks = 18; // Number of stacks (divisions around the major radius)

		// Generate the vertices for the torus
		for (int i = 0; i < numTorusStacks; ++i)
		{
			float theta = i * 2.0f * PI / numTorusStacks; // Angle around the major radius
			for (int j = 0; j < numTorusSlices; ++j)
			{
				float phi = j * 2.0f * PI / numTorusSlices; // Angle around the minor radius

				// Parametric equations for a torus:
				float x = (R + r * cos(phi)) * cos(theta);  // x = (major radius + minor radius * cos(phi)) * cos(theta)
				float y = (R + r * cos(phi)) * sin(theta);  // y = (major radius + minor radius * cos(phi)) * sin(theta)
				float z = r * sin(phi);                     // z = minor radius * sin(phi)

				modelData->vertexData.push_back(vec3(x, y, z)); // Store the vertex
				modelData->vertex_colors.push_back(vec3(1.0f, 0.5f, 0.0f)); // Color the vertices (orange for example)
			}
		}

		// Generate the indices for the torus
		for (int i = 0; i < numTorusStacks; ++i)
		{
			for (int j = 0; j < numTorusSlices; ++j)
			{
				// Vertices around the current stack
				int first = i * numTorusSlices + j;
				int second = ((i + 1) % numTorusStacks) * numTorusSlices + j;
				int next = (j + 1) % numTorusSlices;
				int third = ((i + 1) % numTorusStacks) * numTorusSlices + next;

				// First triangle
				modelData->indexData.push_back(first);
				modelData->indexData.push_back(second);
				modelData->indexData.push_back(third);

				// Second triangle
				modelData->indexData.push_back(first);
				modelData->indexData.push_back(third);
				modelData->indexData.push_back(first + 1);
			}
		}
		break;
	}

	model->SetModelData(*modelData);
}

static void createMeshesFromFBX(const aiScene& scene, std::vector<std::shared_ptr<Model>>& models) {
	
	models.resize(scene.mNumMeshes);

	std::vector<std::shared_ptr<ModelData>> modelsData;
	modelsData.resize(scene.mNumMeshes);

	for (unsigned int i = 0; i < scene.mNumMeshes; i++) {
		aiMesh* mesh = scene.mMeshes[i];
		modelsData[i] = std::make_shared<ModelData>();
		models[i] = std::make_shared<Model>();
		models[i]->SetMeshName(mesh->mName.C_Str());
		models[i]->SetMaterialIndex(mesh->mMaterialIndex);

		for (unsigned int j = 0; j < mesh->mNumVertices; j++) {

			// Coordenadas de los vértices
			aiVector3D vertex = mesh->mVertices[j];
			vec3 aux = vec3(vertex.x, vertex.y, vertex.z);
			modelsData[i]->vertexData.push_back(aux);

			// Coordenadas UV (si existen)
			if (mesh->mTextureCoords[0]) {  // Comprueba si hay UVs
				aiVector3D uv = mesh->mTextureCoords[0][j];
				aux.x = uv.x;  // Solo X y Y
				aux.y = uv.y;
			}
			modelsData[i]->vertex_texCoords.push_back(aux);

			if (mesh->HasNormals()) {  // Verifica si hay normales
				aiVector3D normal = mesh->mNormals[j];
				vec3 auxNormal(normal.x, normal.y, normal.z);
				modelsData[i]->vertex_normals.push_back(auxNormal);
			}

			if (mesh->HasVertexColors(0)) {  // Verifica si hay colores
				aiColor4D color = mesh->mColors[0][j];
				vec3 auxColor(color.r, color.g, color.b);
				modelsData[i]->vertex_colors.push_back(auxColor);
			}

		}

		for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
			aiFace face = mesh->mFaces[j];
			for (unsigned int k = 0; k < face.mNumIndices; k++) {
				modelsData[i]->indexData.push_back(face.mIndices[k]);
			}
		}

		models[i]->SetModelData(*modelsData[i]);
	}
}

static void createMaterialsFromFBX(const aiScene& scene, const std::filesystem::path& basePath, std::vector<std::shared_ptr<Material>>& materials) {

	materials.resize(scene.mNumMaterials);

	for (unsigned int i = 0; i < scene.mNumMaterials; ++i) {
		const auto* fbx_material = scene.mMaterials[i];
		materials[i] = std::make_shared<Material>();

		if (fbx_material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString texturePath;
			fbx_material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
			const string textureFileName = std::filesystem::path(texturePath.C_Str()).filename().string();
			materials[i]->m_TexturePath = (basePath / textureFileName).string();

			auto uWrapMode = aiTextureMapMode_Wrap;
			auto vWrapMode = aiTextureMapMode_Wrap;
			fbx_material->Get(AI_MATKEY_MAPPINGMODE_U_DIFFUSE(0), uWrapMode);
			fbx_material->Get(AI_MATKEY_MAPPINGMODE_V_DIFFUSE(0), vWrapMode);
			assert(uWrapMode == aiTextureMapMode_Wrap);
			assert(vWrapMode == aiTextureMapMode_Wrap);

			unsigned int flags = 0;
			fbx_material->Get(AI_MATKEY_TEXFLAGS_DIFFUSE(0), flags);
			assert(flags == 0);
		}
	}
}

static mat4 aiMat4ToMat4(const aiMatrix4x4& aiMat) {
	mat4 mat;
	for (int c = 0; c < 4; ++c) for (int r = 0; r < 4; ++r) mat[c][r] = aiMat[r][c];
	return mat;
}

std::shared_ptr<GameObject> graphicObjectFromNode(const aiScene& scene, const aiNode& node, const vector<shared_ptr<Model>>& meshes, const vector<shared_ptr<Material>>& materials) {

	std::shared_ptr<GameObject> obj = std::make_shared<GameObject>("scene.mName.C_Str()");

	obj->GetComponent<Transform>()->mat() = aiMat4ToMat4(node.mTransformation);
	obj->GetComponent<Transform>()->updateGlobalMatrix();

	// Inicializa una bounding box combinada vacía
	BoundingBox combinedBoundingBox;

	for (unsigned int i = 0; i < node.mNumMeshes; ++i) {
		const auto meshIndex = node.mMeshes[i];
		const auto& mesh = meshes[meshIndex];

		obj->name() = meshes[meshIndex].get()->GetMeshName();

		obj->AddComponent<Mesh>();
		obj->GetComponent<Mesh>()->setModel(mesh);

		obj->AddComponent<Material>();
		obj->GetComponent<Material>()->m_Texture = std::make_unique<Texture>(materials[mesh->GetMaterialIndex()]->m_TexturePath);
		obj->GetComponent<Material>()->m_Shader = std::make_unique<Shader>("Assets/Shaders/Basic.shader");
		obj->GetComponent<Mesh>()->loadToOpenGL();

		BoundingBox meshBBox;

		meshBBox.min = meshes[meshIndex].get()->GetModelData().vertexData.front();
		meshBBox.max = meshes[meshIndex].get()->GetModelData().vertexData.front();

		for (const auto& v : meshes[meshIndex].get()->GetModelData().vertexData) {
			meshBBox.min = glm::min(meshBBox.min, glm::dvec3(v));
			meshBBox.max = glm::max(meshBBox.max, glm::dvec3(v));
		}

		auto vertices = meshBBox.vertices();
		for (auto& v : vertices) v = obj->AddComponent<Transform>()->mat() * vec4(v, 1);
		combinedBoundingBox = BoundingBox(vertices.data(), vertices.size());
		combinedBoundingBox = obj->AddComponent<Transform>()->mat() * combinedBoundingBox;
	}

	for (unsigned int i = 0; i < node.mNumChildren; ++i) {
		auto child = graphicObjectFromNode(scene, *node.mChildren[i], meshes, materials);
		obj->addChild(child);

		combinedBoundingBox = combinedBoundingBox + child->getBoundingBox();
	}

	obj->setBoundingBox(combinedBoundingBox);

	return obj;
}

std::shared_ptr<GameObject> ModelLoader::loadFromFile(const std::string& filename)
{
	// Derive the custom format file path
	std::filesystem::path filePath = filename;
	std::filesystem::path customFilePath = "Library/Models/" + filePath.string() + ".model";

	if (std::filesystem::exists(customFilePath)) {
		// Load from the custom format
		std::cout << "Loading from custom format: " << customFilePath << std::endl;
		return loadFromCustomFormat(customFilePath.string());
	}
	else {
		// Load the FBX file
		const aiScene* scene = aiImportFile(filename.c_str(),
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_GenUVCoords |
			aiProcess_TransformUVCoords |
			aiProcess_FlipUVs);

		if (!scene) {
			LOG(LogType::LOG_WARNING, "Couldn't find %s", filename.c_str());
			return nullptr;
		}

		// Create models and materials from FBX
		std::vector<std::shared_ptr<Model>> models;
		createMeshesFromFBX(*scene, models);
		std::vector<std::shared_ptr<Material>> materials;
		createMaterialsFromFBX(*scene, std::filesystem::absolute(filename).parent_path(), materials);

		// Create the GameObject hierarchy
		std::shared_ptr<GameObject> fbx_obj = graphicObjectFromNode(*scene, *scene->mRootNode, models, materials);
		aiReleaseImport(scene);

		// Assign name and return
		fbx_obj->name() = std::filesystem::path(filename).stem().string();
		return fbx_obj;
	}
}

void ModelLoader::saveFBXToCustomFormat(const std::string& fbxFilePath, const std::string& outputFilePath) {
	const aiScene* scene = aiImportFile(fbxFilePath.c_str(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_GenUVCoords |
		aiProcess_TransformUVCoords |
		aiProcess_FlipUVs);

	if (!scene) {
		std::cerr << "Error: Unable to load FBX file: " << fbxFilePath << std::endl;
		return;
	}

	std::ofstream file(outputFilePath, std::ios::binary);
	if (!file) {
		std::cerr << "Error: Unable to open file for writing: " << outputFilePath << std::endl;
		aiReleaseImport(scene);
		return;
	}

	// Write the number of models and materials
	size_t modelCount = scene->mNumMeshes;
	size_t materialCount = scene->mNumMaterials;
	file.write(reinterpret_cast<const char*>(&modelCount), sizeof(size_t));
	file.write(reinterpret_cast<const char*>(&materialCount), sizeof(size_t));

	// Write model data
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[i];

		size_t vertexCount = mesh->mNumVertices;
		size_t indexCount = mesh->mNumFaces * 3; // Assuming all faces are triangles
		size_t uvCount = mesh->mNumVertices;
		size_t normalCount = mesh->mNumVertices;
		size_t colorCount = mesh->mNumVertices;

		// Write size data for the current model
		file.write(reinterpret_cast<const char*>(&vertexCount), sizeof(size_t));
		file.write(reinterpret_cast<const char*>(&indexCount), sizeof(size_t));
		file.write(reinterpret_cast<const char*>(&uvCount), sizeof(size_t));
		file.write(reinterpret_cast<const char*>(&normalCount), sizeof(size_t));
		file.write(reinterpret_cast<const char*>(&colorCount), sizeof(size_t));

		// Write vertex data
		for (unsigned int j = 0; j < vertexCount; ++j) {
			aiVector3D vertex = mesh->mVertices[j];
			vec3 pos(vertex.x, vertex.y, vertex.z);
			file.write(reinterpret_cast<const char*>(&pos), sizeof(vec3));
		}

		// Write index data
		for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
			aiFace face = mesh->mFaces[j];
			for (unsigned int k = 0; k < face.mNumIndices; ++k) {
				file.write(reinterpret_cast<const char*>(&face.mIndices[k]), sizeof(unsigned int));
			}
		}

		// Write UV data if it exists
		if (mesh->mTextureCoords[0]) {
			for (unsigned int j = 0; j < uvCount; ++j) {
				aiVector3D uv = mesh->mTextureCoords[0][j];
				vec2 texCoord(uv.x, uv.y);
				file.write(reinterpret_cast<const char*>(&texCoord), sizeof(vec2));
			}
		}

		// Write normal data if it exists
		if (mesh->HasNormals()) {
			for (unsigned int j = 0; j < normalCount; ++j) {
				aiVector3D normal = mesh->mNormals[j];
				vec3 norm(normal.x, normal.y, normal.z);
				file.write(reinterpret_cast<const char*>(&norm), sizeof(vec3));
			}
		}

		// Write color data if it exists
		if (mesh->HasVertexColors(0)) {
			for (unsigned int j = 0; j < colorCount; ++j) {
				aiColor4D color = mesh->mColors[0][j];
				vec3 col(color.r, color.g, color.b);
				file.write(reinterpret_cast<const char*>(&col), sizeof(vec3));
			}
		}
	}

	// Write material data
	for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
		const aiMaterial* mat = scene->mMaterials[i];
		aiString texturePath;
		mat->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);

		size_t texturePathLength = texturePath.length;
		file.write(reinterpret_cast<const char*>(&texturePathLength), sizeof(size_t));
		file.write(texturePath.C_Str(), texturePathLength);
	}

	file.close();
	aiReleaseImport(scene);

	std::cout << "FBX data saved to " << outputFilePath << std::endl;
}


std::shared_ptr<GameObject> ModelLoader::loadFromCustomFormat(const std::string& filePath) {
	std::ifstream file(filePath, std::ios::binary);
	if (!file) {
		std::cerr << "Error: Unable to open custom format file for reading: " << filePath << std::endl;
		return nullptr;
	}

	// Read number of models and materials
	size_t modelCount = 0, materialCount = 0;
	file.read(reinterpret_cast<char*>(&modelCount), sizeof(size_t));
	file.read(reinterpret_cast<char*>(&materialCount), sizeof(size_t));

	std::cout << "Model count: " << modelCount << std::endl;
	std::cout << "Material count: " << materialCount << std::endl;

	// Load models
	std::vector<std::shared_ptr<Model>> models(modelCount);
	for (size_t i = 0; i < modelCount; ++i) {
		models[i] = std::make_shared<Model>();
		auto& modelData = models[i]->GetModelData();

		size_t vertexCount = 0, indexCount = 0, uvCount = 0, normalCount = 0, colorCount = 0;

		// Read data counts
		file.read(reinterpret_cast<char*>(&vertexCount), sizeof(size_t));
		file.read(reinterpret_cast<char*>(&indexCount), sizeof(size_t));
		file.read(reinterpret_cast<char*>(&uvCount), sizeof(size_t));
		file.read(reinterpret_cast<char*>(&normalCount), sizeof(size_t));
		file.read(reinterpret_cast<char*>(&colorCount), sizeof(size_t));

		std::cout << "Model " << i << " - Vertex count: " << vertexCount << std::endl;
		std::cout << "Model " << i << " - Index count: " << indexCount << std::endl;

		if (file.fail()) {
			std::cerr << "Error reading data for model " << i << std::endl;
			return nullptr;
		}

		std::cout << "File position after reading size data for model " << i << ": " << file.tellg() << std::endl;

		// Read vertex data
		modelData.vertexData.resize(vertexCount);
		file.read(reinterpret_cast<char*>(modelData.vertexData.data()), vertexCount * sizeof(vec3));

		// Read index data
		modelData.indexData.resize(indexCount);
		file.read(reinterpret_cast<char*>(modelData.indexData.data()), indexCount * sizeof(unsigned int));

		// Read texture coordinates
		modelData.vertex_texCoords.resize(uvCount);
		file.read(reinterpret_cast<char*>(modelData.vertex_texCoords.data()), uvCount * sizeof(vec2));

		// Read normals
		modelData.vertex_normals.resize(normalCount);
		file.read(reinterpret_cast<char*>(modelData.vertex_normals.data()), normalCount * sizeof(vec3));

		// Read vertex colors
		modelData.vertex_colors.resize(colorCount);
		file.read(reinterpret_cast<char*>(modelData.vertex_colors.data()), colorCount * sizeof(vec3));

		std::streampos currentPosition = file.tellg();
		std::streampos modelDataSize = currentPosition - static_cast<std::streampos>(sizeof(size_t) * 5);

		if (i < modelCount - 1) {
			file.seekg(modelDataSize, std::ios::cur);
			std::cout << "File position after seeking for next model: " << file.tellg() << std::endl;
		}

		// Debug print to verify if the read operation was successful for the current model
		std::cout << "Finished reading model " << i << std::endl;
	}

	// Load materials
	std::vector<std::shared_ptr<Material>> materials(materialCount);
	for (auto& material : materials) {
		material = std::make_shared<Material>();
		size_t texturePathLength = 0;

		// Read texture path
		file.read(reinterpret_cast<char*>(&texturePathLength), sizeof(size_t));
		if (texturePathLength > 0) {
			std::string texturePath(texturePathLength, '\0');
			file.read(&texturePath[0], texturePathLength);
			material->m_TexturePath = texturePath;
		}
	}

	// Load GameObject hierarchy
	std::function<std::shared_ptr<GameObject>()> loadGameObject = [&file, &loadGameObject, &models, &materials]() {
		size_t nameLength = 0;
		file.read(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
		std::string objectName(nameLength, '\0');
		file.read(&objectName[0], nameLength);

		auto gameObject = std::make_shared<GameObject>(objectName);

		// Load transform
		mat4 transformMatrix;
		file.read(reinterpret_cast<char*>(&transformMatrix), sizeof(mat4));
		gameObject->GetComponent<Transform>()->mat() = transformMatrix;

		// Load attached mesh (if any)
		bool hasMesh = false;
		file.read(reinterpret_cast<char*>(&hasMesh), sizeof(bool));
		if (hasMesh) {
			size_t modelIndex = 0;
			file.read(reinterpret_cast<char*>(&modelIndex), sizeof(size_t));
			gameObject->AddComponent<Mesh>()->setModel(models[modelIndex]);

			// Attach material
			size_t materialIndex = 0;
			file.read(reinterpret_cast<char*>(&materialIndex), sizeof(size_t));
			gameObject->AddComponent<Material>()->m_Texture = std::make_unique<Texture>(materials[materialIndex]->m_TexturePath);
		}

		// Load children
		size_t childCount = 0;
		file.read(reinterpret_cast<char*>(&childCount), sizeof(size_t));
		for (size_t i = 0; i < childCount; ++i) {
			gameObject->addChild(loadGameObject());
		}

		return gameObject;
		};

	auto rootObject = loadGameObject();

	file.close();
	return rootObject;
}