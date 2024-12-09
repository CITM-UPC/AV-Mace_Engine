#pragma once

#include <iostream>

#include "Component.h"
#include "Texture.h"
#include "Shader.h"

class Material :  public Component
{
public:
	explicit Material(GameObject* owner) : Component(owner) {}
	Material(GameObject* owner, const std::string& texturePath, const std::string& shaderPath) : Component(owner), m_Texture(std::make_unique<Texture>(texturePath)), m_Shader(std::make_unique<Shader>(shaderPath)), m_TexturePath(texturePath) {}

	std::unique_ptr<Texture> m_Texture; //share_pointer
	std::unique_ptr<Shader> m_Shader; //share_pointer
	string m_TexturePath;
};