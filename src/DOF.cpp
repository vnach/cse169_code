#include "DOF.h"

DOF::DOF()
{
	value = 0;
	min = 0;
	max = 0;
}

DOF::~DOF()
{

}

void DOF::setValue(float val)
{
	value = val;
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
