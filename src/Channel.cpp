#include "Channel.h"
#include <iostream>

Channel::Channel()
{

}

float Channel::Evaluate(float time)
{		
	//std::cout << "Channel Extrapolation: " << extrapIn << std::endl;

	if (time < keyframes.at(0)->time)
	{
		float delta_t = time - keyframes.at(0)->time;
		//extrapolation in (- time diff)
		if (strcmp(this->extrapIn, "linear") == 0)
		{
			return delta_t * -1 * keyframes.at(0)->tangentOut;
		}
		else if (strcmp(extrapIn, "cycle") == 0)
		{
			//t' = a + t%(b-a)
			float channelTime = keyframes.at(keyframes.size()-1)->time - keyframes.at(0)->time;
			delta_t = keyframes.at(0)->time + std::fmod(-1* time + keyframes.at(0)->time, channelTime);
			return LinearSearch(delta_t);
		}
		else if (strcmp(extrapIn, "cycle_offset") == 0)
		{
			//t' = a + t%(b-a)
			float channelTime = keyframes.at(keyframes.size()-1)->time - keyframes.at(0)->time;
			delta_t = keyframes.at(0)->time + std::fmod(-1* time + keyframes.at(0)->time, channelTime);
			int cycles = time / channelTime;
			float offset = cycles * (keyframes.back()->value - keyframes.front()->value);
			return offset + LinearSearch(delta_t);
		}
		else if (strcmp(extrapIn, "bounce") == 0)
		{
			//confusion
			//TODO
		}
		else {
			return keyframes.at(0)->value; //hold first value
		}
		
	}
	else if (time > keyframes.at(keyframes.size()-1)->time)
	{
		//extrapolation out (+ time diff)
		//std::cout << "Channel Extrapolation: " << this->extrapIn << std::endl;
		float delta_t = time - keyframes.at(keyframes.size() - 1)->time;
		if (strcmp(extrapOut, "linear") == 0)
		{
			return delta_t * keyframes.at(keyframes.size()-1)->tangentIn;
		}
		else if (strcmp(extrapOut, "cycle") == 0)
		{
			float channelTime = keyframes.at(keyframes.size()-1)->time - keyframes.at(0)->time;
			delta_t = keyframes.at(0)->time + std::fmod(time - keyframes.at(0)->time, channelTime);
			return LinearSearch(delta_t);
		}
		else if (strcmp(extrapOut, "cycle_offset") == 0)
		{
			float channelTime = keyframes.at(keyframes.size()-1)->time - keyframes.at(0)->time;
			delta_t = keyframes.at(0)->time + std::fmod(time - keyframes.at(0)->time, channelTime);
			int cycles = time / channelTime;
			float offset = cycles * (keyframes.back()->value - keyframes.front()->value);
			return offset + LinearSearch(delta_t);
		}
		else if (strcmp(extrapOut, "bounce") == 0)
		{
			//confusion
			//TODO
		}
		else {
			return keyframes.at(keyframes.size() - 1)->value; //hold last value
		}
		
	}
	return LinearSearch(time);
}

float Channel::LinearSearch(float time)
{
	//else iterate through keyframes
	//std::cout << "time : " << time << std::endl;
	//std::cout << "channel # " << channelNum << std::endl;
	for (int i = 0; i < keyframes.size(); i++)
	{
		if (keyframes.at(i)->time == time)
		{
			return keyframes.at(i)->value;
		}
		else if (i + 1 < keyframes.size())
		{
			if (time > keyframes.at(i)->time && time < keyframes.at(i + 1)->time)
			{
				//calculate cubic span
				return keyframes.at(i)->Evaluate(time, keyframes.at(i)->time, keyframes.at(i + 1)->time);
			}
		}
	}
}

bool Channel::Load(Tokenizer& t)
{
	t.FindToken("{");
	t.SkipLine();
	while (1)
	{
		char temp[256];
		t.GetToken(temp);
		if (strcmp(temp, "extrapolate") == 0)
		{
			t.GetToken(extrapIn);
			t.GetToken(extrapOut);
			continue;
		}
		else if (strcmp(temp, "keys") == 0)
		{
			numKeys = t.GetFloat();
			t.FindToken("{");
			t.SkipLine();
			for (int i = 0; i < numKeys; i++)
			{
				Keyframe* k = new Keyframe();
				k->time = t.GetFloat();
				k->value = t.GetFloat();
				t.GetToken(k->ruleIn);
				t.GetToken(k->ruleOut);

				keyframes.push_back(k);
				//t.SkipLine();
			}
			//t.FindToken("}");
			t.SkipLine();
			continue;
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

void Channel::PrecomputeTangents()
{
	for (int i = 0; i < keyframes.size(); i++)
	{
		Keyframe* k = keyframes.at(i);
		if (strcmp(k->ruleIn, "smooth") == 0)
		{
			if (i - 1 >= 0 && i + 1 < keyframes.size())
			{
				float numerator = keyframes.at(i + 1)->value - keyframes.at(i - 1)->value;
				float denominator = keyframes.at(i + 1)->time - keyframes.at(i - 1)->time;
				k->tangentIn = numerator / denominator;
				k->tangentOut = numerator / denominator;
			}
			else if (i == 0 && i + 1 < keyframes.size()) {
				float numerator = keyframes.at(i + 1)->value - k->value;
				float denominator = keyframes.at(i + 1)->time - k->time;
				k->tangentOut = numerator / denominator;
				keyframes.at(i + 1)->tangentIn = numerator / denominator; //v(i+1) in
			}
			else if (i == keyframes.size() - 1 && i - 1 >= 0)
			{
				float numerator = k->value - keyframes.at(i - 1)->value;
				float denominator = k->time - keyframes.at(i - 1)->time;
				k->tangentIn = numerator / denominator;
				keyframes.at(i - 1)->tangentOut = numerator / denominator; //v(i-1) out
			}
		}
		else if (strcmp(k->ruleIn, "flat") == 0)
		{
			k->tangentIn = 0;
			k->tangentOut = 0;
		} 
		else if (strcmp(k->ruleIn, "linear") == 0)
		{
			if ((i+1) < keyframes.size())
			{
				float numerator = keyframes.at(i + 1)->value - k->value;
				float denominator = keyframes.at(i + 1)->time - k->time;
				k->tangentOut = numerator / denominator; //v(i) out
				keyframes.at(i+1)->tangentIn = numerator / denominator; //v(i+1) in
			}
		} 
		else {
			k->tangentIn = std::atof(k->ruleIn);
			k->tangentOut = std::atof(k->ruleOut);
		}
		
	}
}

void Channel::PrecomputeCubics()
{
	glm::mat4 cubicMat = glm::mat4(	2, -3, 0, 1,
									-2, 3, 0, 0,
									1, -2, 1, 0,
									1, -1, 0, 0);

	for (int i = 0; i < keyframes.size()-1; i++)
	{
		glm::vec4 vals = glm::vec4(	keyframes.at(i)->value,
									keyframes.at(i + 1)->value, 
									(keyframes.at(i + 1)->time - keyframes.at(i)->time) * keyframes.at(i)->tangentOut, 
									(keyframes.at(i + 1)->time - keyframes.at(i)->time) * keyframes.at(i+1)->tangentIn);

		glm::vec4 coeffs = cubicMat * vals;

		keyframes.at(i)->A = coeffs.x;
		keyframes.at(i)->B = coeffs.y;
		keyframes.at(i)->C = coeffs.z;
		keyframes.at(i)->D = coeffs.w;
	}
}






