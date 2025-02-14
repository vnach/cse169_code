#pragma once

#include "core.h"
#include "Joint.h"

class Skeleton {
public:
	Joint* root;
	std::vector<Joint*> joints;

	Skeleton();
	~Skeleton();

	bool Load(const char* file);
	void Update();
	void Draw(glm::mat4 camMatrix, GLuint shader);
	glm::mat4 GetWorldMatrix(int joint);
	std::vector<Joint*> getJoints();
};