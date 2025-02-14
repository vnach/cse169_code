#pragma once

#include "core.h"
#include <vector>
#include "Animation.h"
#include "Skeleton.h"
#include <Joint.h>

class Player {

	float time;
	Animation* clip;
	Skeleton* skel;
	float currentPose;

public:

	void setClip(Animation* c, Skeleton* s)
	{
		clip = c;
		skel = s;
	}

	const float getCurrentPose()
	{
		return currentPose;
	}

	void Update(float deltaTime)
	{
		time = time + deltaTime;
		clip->Evaluate(time);

		//std::cout << "Channel Vals: " << clip->channelValues.at(6) << std::endl;

		float x = clip->channelValues.at(0);
		float y = clip->channelValues.at(1);
		float z = clip->channelValues.at(2);
		skel->root->offset = glm::vec3(x, y, z);
		//skel->root->worldMat = glm::translate(skel->root->worldMat, glm::vec3(x, y, z));
		//skel->root->localMat = glm::translate(skel->root->localMat, glm::vec3(x, y, z));

		std::cout << "time: " << time << " z val : " << z << std::endl;
		
		
		
		for (int i = 0; i < skel->joints.size(); i++)
		{
			Joint* j = skel->joints.at(i);
			j->dofs.at(0)->setValue(clip->channelValues.at((3 * i) + 3));
			j->dofs.at(1)->setValue(clip->channelValues.at((3 * i) + 4));
			j->dofs.at(2)->setValue(clip->channelValues.at((3 * i) + 5));
			
			float xDOFVal = glm::clamp(clip->channelValues.at((3 * i) + 3), skel->joints.at(i)->rotxlimit.x, skel->joints.at(i)->rotxlimit.y);
			float yDOFVal = glm::clamp(clip->channelValues.at((3 * i) + 4), skel->joints.at(i)->rotylimit.x, skel->joints.at(i)->rotylimit.y);
			float zDOFVal = glm::clamp(clip->channelValues.at((3 * i) + 5), skel->joints.at(i)->rotzlimit.x, skel->joints.at(i)->rotzlimit.y);
		
			//skel->joints.at(i)->dofs.at(0)->setValue(clip->channelValues.at((3 * i) + 3));
			//skel->joints.at(i)->dofs.at(1)->setValue(clip->channelValues.at((3 * i) + 4));
			//skel->joints.at(i)->dofs.at(2)->setValue(clip->channelValues.at((3 * i) + 5));


			//std::cout << "joints at " << i << "_X: " << skel->joints.at(i)->dofs.at(0)->getValue() << std::endl;
			//std::cout << "joints at " << i << "_Y: " << skel->joints.at(i)->dofs.at(1)->getValue() << std::endl;
			//std::cout << "joints at " << i << "_Z: " << skel->joints.at(i)->dofs.at(2)->getValue() << std::endl;
		
		}
		
		
		
	}
};