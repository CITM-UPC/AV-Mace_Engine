#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <queue>

void Transform::translate(float v[])
{
    pos() = glm::vec3(v[0], v[1], v[2]);
    updateGlobalMatrix();
}

void Transform::rotate(const vec3& eulerAngles)
{
	vec3 currentTransform = local_mat[3]; // Translation is stored in the last column
	vec3 currentScale = glm::vec3(glm::length(local_mat[0]), glm::length(local_mat[1]), glm::length(local_mat[2])); // Scale is the length of the columns;

	_rot = glm::quat(eulerAngles);
    
	local_mat = glm::mat4(1.0f);
	local_mat[0] = glm::vec4(currentScale.x, 0.0f, 0.0f, 0.0f);
	local_mat[1] = glm::vec4(0.0f, currentScale.y, 0.0f, 0.0f);
	local_mat[2] = glm::vec4(0.0f, 0.0f, currentScale.z, 0.0f);

	local_mat = mat4(glm::mat4_cast(_rot)) * local_mat;
	local_mat[3] = glm::vec4(currentTransform, 1.0f);

	updateGlobalMatrix();
}

void Transform::scale(const vec3& s)
{
    local_mat = glm::scale(local_mat, s);
    updateGlobalMatrix();
}

void Transform::updateGlobalMatrix()
{
    std::queue<Transform*> toUpdate;
	isDirty = true;
    toUpdate.push(this);

    while (!toUpdate.empty()) {
        Transform* current = toUpdate.front();
        toUpdate.pop();

        if (current->isDirty) 
        {
            if (current->owner()->hasParent()) {
                current->global_mat = current->owner()->parent()->GetComponent<Transform>().glob_mat() * current->local_mat;
            }
            else {
                current->global_mat = current->local_mat;
            }
            current->isDirty = false; 

            for (auto& child : current->owner()->children()) 
            {
                child.GetComponent<Transform>().isDirty = true;
                toUpdate.push(&child.GetComponent<Transform>());
            }
        }
    }
}
