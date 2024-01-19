#pragma once

#include "core.h"

class DOF {
private:
	float value, min, max;

public:
	DOF();
	~DOF();

	void setValue(float val);
	float getValue();
	void setMinMax(float setMin, float setMax);
};