#pragma once

#include "core.h"
#include <vector>
#include "Channel.h"


class Animation {
public:
	std::vector<Channel*> channels;
	float timeStart;
	float timeEnd;
	int numChannels;
	std::vector<float> channelValues;

	Animation();
	~Animation();

	bool Load(const char* file);
	void Evaluate(float time);
	//void Draw(glm::mat4 camMatrix, GLuint shader);
};