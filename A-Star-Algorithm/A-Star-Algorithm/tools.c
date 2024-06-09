#include "tools.h"

sfTime sftime;
sfClock* sfclock;

void initTools()
{
	sfclock = sfClock_create();
	srand(time(NULL));
}

void restartClock()
{
	sftime = sfClock_restart(sfclock);
}

float getDeltaTime()
{
	float dt = sfTime_asSeconds(sftime);
	if (dt > 0.1f)
		return 0.1f;

	return dt;
}

sfVector2f vector2f(float _x, float _y)
{
	sfVector2f v = { _x, _y };
	return v;
}

sfVector2i vector2i(int _x, int _y)
{
	sfVector2i v = { _x, _y };
	return v;
}

sfVector2f addVectorsf(sfVector2f _v1, sfVector2f _v2)
{
	sfVector2f v;
	v.x = _v1.x + _v2.x;
	v.y = _v1.y + _v2.y;
	return v;
}

sfVector2i addVectorsi(sfVector2i _v1, sfVector2i _v2)
{
	sfVector2i v;
	v.x = _v1.x + _v2.x;
	v.y = _v1.y + _v2.y;
	return v;
}

sfVector2f MultiplyVector2f(sfVector2f _v, float _m)
{
	_v.x *= _m;
	_v.y *= _m;
	return _v;
}

sfVector2i MultiplyVector2i(sfVector2i _v, int _m)
{
	_v.x *= _m;
	_v.y *= _m;
	return _v;
}

sfVector2f V2iToV2f(sfVector2i _v)
{
	sfVector2f v;
	v.x = (float)_v.x;
	v.y = (float)_v.y;
	return v;
}

sfVector2i V2fToV2i(sfVector2f _v)
{
	sfVector2i v;
	v.x = (int)_v.x;
	v.y = (int)_v.y;
	return v;
}

sfColor color(sfUint8 _r, sfUint8 _g, sfUint8 _b)
{
	sfColor c;
	c.r = _r;
	c.g = _g;
	c.b = _b;
	c.a = 255;
	return c;
}

sfFloatRect FloatRect(float _left, float _top, float _width, float _height)
{
	sfFloatRect f;
	f.left = _left;
	f.top = _top;
	f.width = _width;
	f.height = _height;
	return f;
}

sfIntRect IntRect(int _left, int _top, int _width, int _height)
{
	sfIntRect i;
	i.left = _left;
	i.top = _top;
	i.width = _width;
	i.height = _height;
	return i;
}
