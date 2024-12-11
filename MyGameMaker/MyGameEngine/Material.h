#ifndef __MATERIAL_H__
#define __MATERIAL_H__
#pragma once

#include "Component.h"

#include <iostream>

#include "Texture.h"
#include "Shader.h"

class Material :  public Component
{
public:
	Material(bool active, GameObject* owner) : Component(active, owner) {}
	Material() = default;
	~Material() = default;
	std::unique_ptr<Texture> m_Texture;
	std::unique_ptr<Shader> m_Shader;
	string m_TexturePath;
};

#endif // !__MATERIAL_H__