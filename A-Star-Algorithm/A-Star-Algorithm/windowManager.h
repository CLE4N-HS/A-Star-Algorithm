#pragma once
#include "tools.h"

typedef struct {
	sfRenderWindow* renderWindow;
	sfRenderTexture* renderTexture;
	sfBool isDone;
}Window;

Window* windowSetup();
void initWindow();
void updateWindow(Window* _window);
void displayWindow(Window* _window);
sfBool isDone(Window* _window);
