#pragma once

#include "core.h"
#include "Skeleton.h"
#include "Vertex.h"
#include "Triangle.h"

class Skin {
public:
	Skeleton* skel;

	//binding space
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> posW;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> normW;

	std::vector<glm::mat4> worldMats;
	std::vector<glm::mat4> bindingMatsInv;
	std::vector<glm::mat4> skinningMats;

	std::vector<Vertex*> vertices;
	std::vector<glm::vec3> triangles;
	std::vector<unsigned int> indices;
	int numVerts;


	GLuint VAO;
	GLuint VBO_positions, VBO_normals, EBO;

	glm::mat4 model;
	glm::vec3 color;

	Skin(Skeleton* s);
	~Skin();

	bool Load(const char* file);
	void Update();
	void Draw(glm::mat4 camMatrix, GLuint shader);
};