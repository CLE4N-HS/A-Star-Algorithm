#include "customMath.h"

float lerp(float _a, float _b, float _t)
{
	return (_b - _a) * _t + _a;
}
