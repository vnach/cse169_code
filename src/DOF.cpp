#include "DOF.h"

DOF::DOF()
{
	value = 0;
	min = -1000.f;
	max = 1000.f;
}

DOF::~DOF()
{

}

void DOF::setValue(float val)
{
	value = glm::clamp(val, min, max);
}

float DOF::getValue()
{
	return value;
}

void DOF::setMinMax(float setMin, float setMax)
{
	min = setMin;
	max = setMax;
}

void DOF::setName(char* name)
{
	dofName = name;
}

char* DOF::getName()
{
	return dofName;
}