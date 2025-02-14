#pragma once

#include "core.h"

class DOF {
private:
	

public:
	DOF();
	~DOF();

	float value;
	float min;
	float max;
	char* dofName;

	void setValue(float val);
	float getValue();
	void setMinMax(float setMin, float setMax);
	void setName(char* name);
	char* getName();
};