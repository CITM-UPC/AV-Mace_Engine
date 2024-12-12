#pragma once

#include "types.h"
#include "Component.h"

class Transform : public Component 
{
	union {
		mat4 local_mat = mat4(1.0f);
		struct {
			vec3 _left; mat4::value_type _left_w;
			vec3 _up; mat4::value_type _up_w;
			vec3 _fwd; mat4::value_type _fwd_w;
			vec3 _pos; mat4::value_type _pos_w;
		};
	};

	union {
		mat4 global_mat = mat4(1.0f);
		struct {
			vec3 global_left; mat4::value_type glob_left_w;
			vec3 global_up; mat4::value_type glob_up_w;
			vec3 global_fwd; mat4::value_type glob_fwd_w;
			vec3 global_pos; mat4::value_type glob_pos_w;
		};
	};

	glm::quat _rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f);    

	bool isDirty = true;

public:
	// Local Transform
	const mat4 mat() const { return local_mat; }
	mat4 &mat() { return local_mat; }
	const auto& left() const { return _left; }
	const auto& right() const { return _left; }
	const auto& up() const { return _up; }
	const auto& fwd() const { return _fwd; }
	const auto* data() const { return &local_mat[0][0]; }
	
	// Global Transfrom
	const mat4 glob_mat() const { return global_mat; }
	mat4 &glob_mat() { return global_mat; }
	const vec3 glob_left() const { return global_left; }
	const vec3 glob_right() const { return global_left; }
	const vec3 glob_up() const { return global_up; }
	const vec3 glob_fwd() const { return global_fwd; }

	//Position
	vec3& pos() { return _pos; }
	const vec3& pos() const { return _pos; }
	vec3& glob_pos() { return global_pos; }

	//Rotation
	glm::quat rot() const { return _rot; }
	vec3 eulerAngles() const { return glm::eulerAngles(_rot); }

	//Scale
	const glm::vec3& scale() const { return _scale; }
	glm::vec3& scale() { return _scale; }

	void translate(const vec3& v) { local_mat = glm::translate(local_mat, v); }
	void translate(float v[]);
	void rotate(const vec3& eulerAngles);
	void rotate(double rads, const vec3& v) { local_mat = glm::rotate(local_mat, rads, v); }
	void scale(const vec3& s);
	void updateGlobalMatrix();

	void setFwd(const vec3& fwd) { _fwd = fwd; }
	void setRigth(const vec3& left) { _left = left; }
	void setUp(const vec3& up) { _up = up; }
	void makeDirty() { isDirty = true; }

	explicit Transform(GameObject* owner) : Component(owner) {};
	Transform(GameObject* owner, const mat4& mat) : Component(owner), local_mat(mat) {};
	Transform& operator*=(const mat4& other) { local_mat *= other;	return *this; }
	Transform& operator*=(const Transform& other) { local_mat *= other.local_mat; return *this; }
};

inline Transform operator*(const mat4& m, const Transform& t) { return Transform(t.owner(), m * t.mat()); }