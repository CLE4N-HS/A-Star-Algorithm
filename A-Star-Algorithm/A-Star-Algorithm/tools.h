#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "SFML/Graphics.h"
#include <time.h>

#define WINDOW_LENGTH 1920
#define WINDOW_HEIGHT 1080

#define NULLVECTOR2F vector2f(0.f, 0.f)
#define NULLVECTOR2I vector2i(0, 0)

void initTools();
void restartClock();
float getDeltaTime();

sfVector2f vector2f(float _x, float _y);
sfVector2i vector2i(int _x, int _y);
sfVector2f addVectorsf(sfVector2f _v1, sfVector2f _v2);
sfVector2i addVectorsi(sfVector2i _v1, sfVector2i _v2);
sfVector2f MultiplyVector2f(sfVector2f _v, float _m);
sfVector2i MultiplyVector2i(sfVector2i _v, int _m);
sfVector2f V2iToV2f(sfVector2i _v);
sfVector2i V2fToV2i(sfVector2f _v);
sfVector2f getfMousePos(sfRenderWindow* _renderWindow);
sfColor color(sfUint8 _r, sfUint8 _g, sfUint8 _b);
