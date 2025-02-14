#include "Animation.h"
#include <Tokenizer.h>
#include <iostream>

Animation::Animation()
{
	numChannels = 0;

}

Animation::~Animation()
{

}

bool Animation::Load(const char* file)
{
	Tokenizer t;
	t.Open(file);
	t.FindToken("{");
	while (1)
	{
		char temp[256];
		t.GetToken(temp);
		if (strcmp(temp, "range") == 0)
		{
			timeStart = t.GetFloat();
			timeEnd = t.GetFloat();
			continue;
		}
		else if (strcmp(temp, "numchannels") == 0)
		{
			numChannels = t.GetFloat();
			continue;
		}
		else if (strcmp(temp, "channel") == 0)
		{
			Channel* c = new Channel();
			c->channelNum = t.GetFloat();
			c->Load(t);
			c->PrecomputeTangents();
			c->PrecomputeCubics();
			channels.push_back(c);
			
			t.FindToken("}");
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
	t.Close();
	return true;
}

void Animation::Evaluate(float time)
{
	channelValues.clear();
	for (int i = 0; i < channels.size(); i++)
	{
		//std::cout << "Channel Extrapolation: " << channels.at(i)->extrapIn << std::endl;
		float val = channels.at(i)->Evaluate(time);
		channelValues.push_back(val);

		//std::cout << "Channel # " << i << " value: " << val << std::endl;
	}
}
