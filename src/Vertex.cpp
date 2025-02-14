#include "Vertex.h"

Vertex::Vertex(glm::vec3 pos, glm::vec3 norm)
{
	position = pos;
	normal = norm;
}

Vertex::~Vertex()
{

}

void Vertex::transPos(std::vector<glm::mat4> skinningMats)
{
	if (skinningMats.size() == 0)
	{
		posW = position;
		return;
	}
	glm::mat4 weightedSum = glm::mat4(0);
	for (int i = 0; i < jointWeights.size(); i++)
	{
		weightedSum = weightedSum + (jointWeights.at(i) * skinningMats.at(jointIndices.at(i)));
	}
	posW = glm::vec3(weightedSum * glm::vec4(position, 1));
}

void Vertex::transNorm(std::vector<glm::mat4> skinningMats)
{
	if (skinningMats.size() == 0)
	{
		normW = normal;
		return;
	}
	glm::mat4 weightedSum = glm::mat4(0);
	for (int i = 0; i < jointWeights.size(); i++)
	{
		weightedSum = weightedSum + (jointWeights.at(i) * skinningMats.at(jointIndices.at(i)));
	}

	glm::mat4 mtxInverseTranspose = glm::inverse(glm::transpose(weightedSum));
	normW = glm::normalize(glm::vec3(mtxInverseTranspose * glm::vec4(normal, 0)));
}