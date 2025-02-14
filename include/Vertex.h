#pragma once

#include "core.h"
#include "Skeleton.h"

class Vertex {
public:
	glm::vec3 position;
	glm::vec3 posW;
	glm::vec3 normal;
	glm::vec3 normW;

	std::vector<int> jointIndices;
	std::vector<float> jointWeights;

	Vertex(glm::vec3 pos, glm::vec3 norm);
	~Vertex();

	void Vertex::transPos(std::vector<glm::mat4> skinningMats);
	void Vertex::transNorm(std::vector<glm::mat4> skinningMats);



};