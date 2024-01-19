#include "Joint.h"
#include "Cube.h"
#include "Camera.h"
#include "Window.h"

Joint::Joint()
{
	//initialize with default values
	offset = glm::vec3(0.f, 0.f, 0.f);

	boxmin = glm::vec3(-0.1f, -0.1f, -0.1f);
	boxmax = glm::vec3(0.1f, 0.1f, 0.1f);

	rotxlimit = glm::vec2(-1000.f, 1000.f);
	rotylimit = glm::vec2(-1000.f, 1000.f);
	rotzlimit = glm::vec2(-1000.f, 1000.f);

	pose = glm::vec3(0.f, 0.f, 0.f);

	localMat = glm::mat4(1.0f);
	worldMat = glm::mat4(1.0f);

	parent = nullptr;
}

Joint::~Joint()
{

}

bool Joint::Load(Tokenizer& t)
{
	t.FindToken("{");
	while (1)
	{
		char temp[256];
		t.GetToken(temp);
		if (strcmp(temp, "offset") == 0)
		{
			offset.x = t.GetFloat();
			offset.y = t.GetFloat();
			offset.z = t.GetFloat();
		}
		else if (strcmp(temp, "boxmin") == 0)
		{
			boxmin.x = t.GetFloat();
			boxmin.y = t.GetFloat();
			boxmin.z = t.GetFloat();
		}
		else if (strcmp(temp, "boxmax") == 0)
		{
			boxmax.x = t.GetFloat();
			boxmax.y = t.GetFloat();
			boxmax.z = t.GetFloat();
		}
		else if (strcmp(temp, "rotxlimit") == 0)
		{
			rotxlimit.x = t.GetFloat();
			rotxlimit.y = t.GetFloat();
		}
		else if (strcmp(temp, "rotylimit") == 0)
		{
			rotylimit.x = t.GetFloat();
			rotylimit.y = t.GetFloat();
		}
		else if (strcmp(temp, "rotzlimit") == 0)
		{
			rotzlimit.x = t.GetFloat();
			rotzlimit.y = t.GetFloat();
		}
		else if (strcmp(temp, "pose") == 0)
		{
			pose.x = t.GetFloat();
			pose.y = t.GetFloat();
			pose.z = t.GetFloat();
		}
		else if (strcmp(temp, "balljoint") == 0)
		{
			Joint* jnt = new Joint();
			jnt->Load(t);
			AddChild(jnt);
		}
		else if (strcmp(temp, "}") == 0)
		{
			cube = new Cube(boxmin, boxmax);
			return true;
		}
		else
		{
			t.SkipLine(); //unrecognized token
		}
	}
}


void Joint::Update(glm::mat4& parentMatrix)
{
	//genrate L matrix
	//generate W matrix = W_parent * L
	//note: GLM is column wise matrix

	float xDOFVal = glm::clamp(pose.x, rotxlimit.x, rotxlimit.y);
	float yDOFVal = glm::clamp(pose.y, rotylimit.x, rotylimit.y);
	float zDOFVal = glm::clamp(pose.z, rotzlimit.x, rotzlimit.y);

	glm::mat4 t_mat = glm::translate(glm::mat4(1.0f), offset);
	glm::mat4 x_rot = glm::rotate(glm::mat4(1.0f), xDOFVal, glm::vec3(1, 0, 0));
	glm::mat4 y_rot = glm::rotate(glm::mat4(1.0f), yDOFVal, glm::vec3(0, 1, 0));
	glm::mat4 z_rot = glm::rotate(glm::mat4(1.0f), zDOFVal, glm::vec3(0, 0, 1));


	localMat = t_mat * z_rot * y_rot * x_rot;

	worldMat = parentMatrix * localMat;

	for each (Joint * child in children)
	{
		child->Update(worldMat);
	}
}

//pass in shader from window class and skeleton class
void Joint::Draw(glm::mat4 camMatrix, GLuint shader)
{
	glm::mat4 viewProjMat = camMatrix * worldMat;

	cube->draw(viewProjMat, shader);

	for each (Joint* child in children)
	{
		child->Draw(camMatrix, shader);
	}
}

void Joint::AddChild(Joint* jnt)
{
	jnt->parent = this;
	children.push_back(jnt);
}

