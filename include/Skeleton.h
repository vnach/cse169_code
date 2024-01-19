#pragma once

#include "core.h"
#include "Joint.h"

class Skeleton {
public:
	Joint* root;

	Skeleton();
	~Skeleton();

	bool Load(const char* file);
	void Update();
	void Draw(glm::mat4 camMatrix, GLuint shader);
};