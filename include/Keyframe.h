#pragma once

#include "core.h"


class Keyframe {
public:

	Keyframe();

	float time;
	float value;
	float tangentIn, tangentOut;
	char ruleIn[256], ruleOut[256]; //tangent rules
	float A, B, C, D; //cubic coefficients;

	float Evaluate(float t, float t0, float t1);

	
};