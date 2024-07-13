#pragma once
#include "tools.h"
#include "windowManager.h"

sfMutex* gameMutex;
sfMutex* fpsMutex;

void initGame(Window* _window);
void updateGame(Window* _window);
void updateGameThread(Window* _window);
void updateFpsThread(Window* _window);
void displayGame(Window* _window);
void updateFPS();
void updateFPSThread2();
void displayFPS(Window* _window);
