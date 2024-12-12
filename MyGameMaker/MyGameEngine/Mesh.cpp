#include "Mesh.h"
#include "Engine.h"
#include "Renderer.h"
#include "Scene.h"
#include "Material.h"

void Mesh::loadToOpenGL()
{
	GLCall(glGenVertexArrays(1, &model.get()->GetModelData().vA));
	GLCall(glBindVertexArray(model.get()->GetModelData().vA));

	//buffer de positions
	GLCall(glGenBuffers(1, &model.get()->GetModelData().vBPosID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, model.get()->GetModelData().vBPosID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, model.get()->GetModelData().vertexData.size() * sizeof(vec3), model.get()->GetModelData().vertexData.data(), GL_STATIC_DRAW));

	//position layout
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(vec3), (const void*)0));

	//buffer de coordenades de textura
	if (model.get()->GetModelData().vertex_texCoords.size() > 0)
	{
		GLCall(glGenBuffers(1, &model.get()->GetModelData().vBTCoordsID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, model.get()->GetModelData().vBTCoordsID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, model.get()->GetModelData().vertex_texCoords.size() * sizeof(vec2), model.get()->GetModelData().vertex_texCoords.data(), GL_STATIC_DRAW));

		//tex coord layout
		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, sizeof(vec2), (const void*)0));
	}

	//buffer de normals
	if (model.get()->GetModelData().vertex_normals.size() > 0)
	{
		GLCall(glGenBuffers(1, &model.get()->GetModelData().vBNormalsID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, model.get()->GetModelData().vBNormalsID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, model.get()->GetModelData().vertex_normals.size() * sizeof(vec3), model.get()->GetModelData().vertex_normals.data(), GL_STATIC_DRAW));

		//normal layout
		GLCall(glEnableVertexAttribArray(2));
		GLCall(glVertexAttribPointer(2, 3, GL_DOUBLE, GL_FALSE, sizeof(vec3), (const void*)0));

		//load normals lines for debugging
		loadNormalsToOpenGL();
		loadFaceNormalsToOpenGL();
	}

	//buffer de index
	GLCall(glCreateBuffers(1, &model.get()->GetModelData().iBID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.get()->GetModelData().iBID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.get()->GetModelData().indexData.size() * sizeof(unsigned int), model.get()->GetModelData().indexData.data(), GL_STATIC_DRAW));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));
	
}

void Mesh::drawModel() const
{
	GLCall(glBindVertexArray(model.get()->GetModelData().vA));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.get()->GetModelData().iBID));

	this->owner()->GetComponent<Material>().m_Shader->Bind();

	if (this->owner()->GetComponent<Material>().m_Texture) {
		this->owner()->GetComponent<Material>().m_Texture->Bind();
		this->owner()->GetComponent<Material>().m_Shader->SetUniformBool("u_HasTexture", true);
		this->owner()->GetComponent<Material>().m_Shader->SetUniform1i("u_Texture", 0);
	}
	else {
		this->owner()->GetComponent<Material>().m_Shader->SetUniformBool("u_HasTexture", false);
	}
	this->owner()->GetComponent<Material>().m_Shader->SetUniformMat4f("u_MVP", (glm::mat4)Engine::Instance().scene->camera()->GetComponent<Camera>().projection() * (glm::mat4)Engine::Instance().scene->camera()->GetComponent<Camera>().view() * (glm::mat4)owner()->GetComponent<Transform>().glob_mat());

	glDrawElements(GL_TRIANGLES, model.get()->GetModelData().indexData.size(), GL_UNSIGNED_INT, nullptr);

	this->owner()->GetComponent<Material>().m_Shader->UnBind();

	if (this->owner()->GetComponent<Material>().m_Texture) {
		this->owner()->GetComponent<Material>().m_Texture->Unbind();
	}

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));

	//drawing normal lines----------------------------------
	if (model.get()->GetModelData().vertex_normals.size() > 0)
	{
		if (debugNormals) drawNormals();
		if (debugFaceNormals) drawFaceNormals();
	}

}

void Mesh::loadNormalsToOpenGL()
{
	float normal_length = 0.1f;  // Ajusta la longitud según sea necesario

	for (unsigned int i = 0; i < model.get()->GetModelData().vertexData.size(); i++) {
		glm::vec3 vertex = model.get()->GetModelData().vertexData[i];
		glm::vec3 normal = model.get()->GetModelData().vertex_normals[i];

		// Agregar el punto inicial (vértice) y el punto final (normal)
		normalLines.push_back(vertex);
		normalLines.push_back(vertex + normal * normal_length);
	}

	// Crear el VBO para las normales
	glGenBuffers(1, &vBNormalsLinesID);
	glBindBuffer(GL_ARRAY_BUFFER, vBNormalsLinesID);
	glBufferData(GL_ARRAY_BUFFER, normalLines.size() * sizeof(glm::vec3), normalLines.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // Desvincular el buffer después de cargar los datos

	m_NormalLinesShader = std::make_unique<Shader>("Assets/Shaders/NormalLines.shader");
}

void Mesh::drawNormals() const
{
	m_NormalLinesShader->Bind();
	m_NormalLinesShader->SetUniformMat4f("u_MVP", (glm::mat4)Engine::Instance().scene->camera()->GetComponent<Camera>().projection() * (glm::mat4)Engine::Instance().scene->camera()->GetComponent<Camera>().view() * (glm::mat4)owner()->GetComponent<Transform>().glob_mat());
	m_NormalLinesShader->SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);

	// Enlaza el VBO de las líneas de normales
	glBindBuffer(GL_ARRAY_BUFFER, vBNormalsLinesID);
	glEnableVertexAttribArray(0); // Usa la ubicación 0 para las posiciones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (const void*)0);

	// Dibuja las líneas de normales
	glDrawArrays(GL_LINES, 0, normalLines.size());

	// Limpieza después de dibujar las líneas
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_NormalLinesShader->UnBind();
}

void Mesh::loadFaceNormalsToOpenGL()
{
	float normal_length = 0.1f;  // Ajusta la longitud según prefieras

	// Recorrer todas las caras (triángulos) de la malla
	for (unsigned int i = 0; i < model.get()->GetModelData().indexData.size(); i += 3) {
		// Obtener los índices de los tres vértices del triángulo
		unsigned int idx0 = model.get()->GetModelData().indexData[i];
		unsigned int idx1 = model.get()->GetModelData().indexData[i + 1];
		unsigned int idx2 = model.get()->GetModelData().indexData[i + 2];

		// Obtener las posiciones de los vértices
		glm::vec3 v0 = model.get()->GetModelData().vertexData[idx0];
		glm::vec3 v1 = model.get()->GetModelData().vertexData[idx1];
		glm::vec3 v2 = model.get()->GetModelData().vertexData[idx2];

		// Calcular el centro del triángulo
		glm::vec3 center = (v0 + v1 + v2) / 3.0f;

		// Calcular la normal de la cara usando el producto cruzado
		glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

		// Agregar el punto inicial (centro) y el punto final (normal)
		faceNormalLines.push_back(center);
		faceNormalLines.push_back(center + normal * normal_length);
	}

	// Crear el VBO para las líneas de las normales por cara
	glGenBuffers(1, &vBfaceNormalLinesID);
	glBindBuffer(GL_ARRAY_BUFFER, vBfaceNormalLinesID);
	glBufferData(GL_ARRAY_BUFFER, faceNormalLines.size() * sizeof(glm::vec3), faceNormalLines.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // Desvincular el buffer después de cargar los datos
}

void Mesh::drawFaceNormals() const {
	m_NormalLinesShader->Bind();
	m_NormalLinesShader->SetUniformMat4f("u_MVP", (glm::mat4)Engine::Instance().scene->camera()->GetComponent<Camera>().projection() * (glm::mat4)Engine::Instance().scene->camera()->GetComponent<Camera>().view() * (glm::mat4)owner()->GetComponent<Transform>().glob_mat());
	m_NormalLinesShader->SetUniform4f("u_Color", 0.0f, 0.0f, 1.0f, 1.0f);

	// Enlaza el VBO de las líneas de normales por cara
	glBindBuffer(GL_ARRAY_BUFFER, vBfaceNormalLinesID);
	glEnableVertexAttribArray(0); // Usa la ubicación 0 para las posiciones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (const void*)0);

	// Dibuja las líneas de normales por cara
	glDrawArrays(GL_LINES, 0, faceNormalLines.size());

	// Limpieza después de dibujar las líneas
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_NormalLinesShader->UnBind();
}