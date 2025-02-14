#pragma once

#include "core.h"
#include "Keyframe.h"
#include <Tokenizer.h>
#include <vector>


class Channel {
public:
	char extrapIn[256];
	char extrapOut[256];
	int numKeys;
	std::vector<Keyframe*> keyframes;
	int channelNum;


	Channel();

	float Evaluate(float time);
	bool Load(Tokenizer& t);
	void PrecomputeTangents();
	void PrecomputeCubics();

private:
	float LinearSearch(float time);
};