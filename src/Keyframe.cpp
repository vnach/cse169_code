#include "Keyframe.h"
#include <iostream>

Keyframe::Keyframe()
{
	tangentIn = 0.0f;
	tangentOut = 0.0f;
	A = 0.0f;
	B = 0.0f;
	C = 0.0f;
	D = 0.0f;
}

float Keyframe::Evaluate(float t, float t0, float t1)
{
	int i = 0;
	float u = (t - t0) / (t1 - t0);
	if (u <= 0 || u >= 1)
	{
		int a = 0;
	}
	float f = D + (u * (C + (u * (B + (u * A)))));

	//std::cout << "Keyframe evaluation for " << i << " :" << f << std::endl;
	return f;
}