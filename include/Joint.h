#pragma once

#include <vector>
#include "core.h"
#include "DOF.h"
#include "Tokenizer.h"
#include "Cube.h"


class Joint {
public:
    
    Cube* cube;

    // Local and World Matrices
    glm::mat4 localMat;
    glm::mat4 worldMat;

    //array of DOFs
    std::vector<DOF> degreesOfFreedom;
    
    //tree data
    Joint* parent;
    std::vector<Joint*> children;
    
    char jointName[256];

    //tokens
    glm::vec3 offset;
    glm::vec3 boxmin;
    glm::vec3 boxmax;
    glm::vec2 rotxlimit; //min, max
    glm::vec2 rotylimit;
    glm::vec2 rotzlimit;
    glm::vec3 pose;

    std::vector<DOF*> dofs;

    Joint();
    ~Joint();

    bool Load(Tokenizer &t);
    void Update(glm::mat4& parentMatrix);
    void Draw(glm::mat4 camMatrix, GLuint shader);
    void AddChild(Joint* jnt);
    void GetJoints(std::vector<Joint*>* joints);

};