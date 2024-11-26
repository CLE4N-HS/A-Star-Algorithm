#include "customMath.h"

float Lerp(float _a, float _b, float _t)
{
	return (_b - _a) * _t + _a;
}

sfVector2f PolarCoords(sfVector2f _startPos, float _distance, float _angle)
{
	return vector2f(_startPos.x + (_distance * cosf(_angle)), _startPos.y + (_distance * sinf(_angle)));
}
