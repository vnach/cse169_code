#include "Skin.h"
#include <iostream>

Skin::Skin(Skeleton* s)
{
	skel = s;
}

Skin::~Skin()
{

}

bool Skin::Load(const char* file)
{
	Tokenizer t;
	t.Open(file);
	while (1)
	{
		char temp[256];
		t.GetToken(temp);
		if (strcmp(temp, "positions") == 0)
		{
			numVerts = t.GetFloat();
			t.FindToken("{");
			int currLine = 0;
			while (currLine < numVerts)
			{
				glm::vec3 pos = glm::vec3(t.GetFloat(), t.GetFloat(), t.GetFloat());
				positions.push_back(pos);
				t.SkipLine();
				currLine++;
				
			}
			t.FindToken("}");
			t.SkipLine();
			continue;
		}
		else if (strcmp(temp, "normals") == 0)
		{
			numVerts = t.GetFloat();
			t.FindToken("{");
			int currLine = 0;
			while (currLine < numVerts)
			{
				glm::vec3 norm = glm::vec3(t.GetFloat(), t.GetFloat(), t.GetFloat());
				normals.push_back(norm);
				t.SkipLine();
				currLine++;
			}
			t.FindToken("}");
			t.SkipLine();
			continue;
		}
		else if (strcmp(temp, "skinweights") == 0)
		{
			numVerts = t.GetFloat();
			t.FindToken("{");
			int currLine = 0;
			while (currLine < numVerts)
			{
				Vertex* v = new Vertex(positions.at(currLine), normals.at(currLine));
				
				int numAttach = t.GetFloat();
				for (int i = 0; i < numAttach; i++)
				{
					v->jointIndices.push_back(t.GetFloat());
					v->jointWeights.push_back(t.GetFloat());
				}
				vertices.push_back(v);
				t.SkipLine();
				currLine++;
			}
			t.FindToken("}");
			t.SkipLine();
			continue;
		}
		else if (strcmp(temp, "triangles") == 0)
		{
			int numTriangles = t.GetFloat();
			t.FindToken("{");
			int currLine = 0;
			while (currLine < numTriangles)
			{
				int v1 = t.GetFloat();
				int v2 = t.GetFloat();
				int v3 = t.GetFloat();
				
				triangles.push_back(glm::vec3(v1, v2, v3));
				indices.push_back(v1);
				indices.push_back(v2);
				indices.push_back(v3);

				t.SkipLine();
				currLine++;
			}

			t.FindToken("}");
			t.SkipLine();
			continue;
		}
		else if (strcmp(temp, "bindings") == 0)
		{
			int linenum = t.GetLineNum();
			int numBindings = t.GetFloat();
			for(int i = 0; i < numBindings; i++)
			{
				t.FindToken("matrix");
				t.SkipLine();
				glm::vec4 c1 = glm::vec4(t.GetFloat(), t.GetFloat(), t.GetFloat(), 0);
				t.SkipLine();
				glm::vec4 c2 = glm::vec4(t.GetFloat(), t.GetFloat(), t.GetFloat(), 0);
				t.SkipLine();
				glm::vec4 c3 = glm::vec4(t.GetFloat(), t.GetFloat(), t.GetFloat(), 0);
				t.SkipLine();
				glm::vec4 c4 = glm::vec4(t.GetFloat(), t.GetFloat(), t.GetFloat(), 1);
				t.SkipLine();
				glm::mat4 bindMat = glm::mat4(c1, c2, c3, c4);
				bindingMatsInv.push_back(glm::inverse(bindMat));
			}

			return true;
		}
		else if (strcmp(temp, "}") == 0)
		{
			return true;
		}
		else
		{
			t.SkipLine(); //unrecognized token
		}
	}
	return true;

}

void Skin::Update()
{
	//std::cout << "positions: " << positions.size() << std::endl;
	//std::cout << "normals " << normals.size() << std::endl;

	skinningMats.clear();
	posW.clear();
	normW.clear();
	
	std::vector<Joint*> joints = skel->joints;
	for (int i = 0; i < joints.size(); i++)
	{
		glm::mat4 worldM = skel->GetWorldMatrix(i);
		skinningMats.push_back(worldM * bindingMatsInv.at(i));
	}
	for (int j = 0; j < vertices.size(); j++)
	{
		Vertex* v = vertices.at(j);
		v->transPos(skinningMats);
		posW.push_back(v->posW);
		v->transNorm(skinningMats);
		normW.push_back(v->normW);
	}
}

void Skin::Draw(glm::mat4 camMatrix, GLuint shader)
{
	// Model matrix.
	model = glm::mat4(1.0f);

	// The color of the cube. Try setting it to something else!
	color = glm::vec3(1.0f, 0.5f, 0.1f);

	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_positions);
	glGenBuffers(1, &VBO_normals);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// Bind to the first VBO - We will use it to store the vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), posW.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO - We will use it to store the normals
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normW.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO and send the data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// actiavte the shader program
	glUseProgram(shader);

	// get the locations and send the uniforms to the shader
	glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&camMatrix);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
	glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

	// Bind the VAO
	glBindVertexArray(VAO);

	// draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	//std::cout << "drew triangle " << std::endl;
	

	// Unbind the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


}



