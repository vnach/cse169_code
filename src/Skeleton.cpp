#pragma once

#include "Skeleton.h"

Skeleton::Skeleton()
{
	root = new Joint();
	root->localMat = glm::mat4(1.0f);
}

Skeleton::~Skeleton()
{
	root = nullptr;
}

bool Skeleton::Load(const char* file)
{
	Tokenizer token;
	token.Open(file);
	token.FindToken("balljoint");

	//Joint* root = new Joint();
	root->Load(token);

	getJoints();

	token.Close();
	return true;	
}

void Skeleton::Update()
{
	root->Update(root->localMat);
}

void Skeleton::Draw(glm::mat4 camMatrix, GLuint shader)
{
	root->Draw(camMatrix, shader);
}

glm::mat4 Skeleton::GetWorldMatrix(int joint)
{
	return joints.at(joint)->worldMat;
}

std::vector<Joint*> Skeleton::getJoints()
{
	joints.push_back(root);
	root->GetJoints(&joints);
	return joints;
}