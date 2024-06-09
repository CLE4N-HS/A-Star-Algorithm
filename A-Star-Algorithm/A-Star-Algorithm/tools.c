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

sfVector2f getfMousePos(sfRenderWindow* _renderWindow)
{
	return V2iToV2f(sfMouse_getPositionRenderWindow(_renderWindow));
}
