#pragma once

#include <vector>

#include "core.h"
#include "Vertex.h"

class Triangle {
private:
    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;

    glm::mat4 model;
    glm::vec3 color;

    
    

public:

    // Triangle Information
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    std::vector<Vertex*> vertices;

    Triangle();
    ~Triangle();

    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    void update();

};