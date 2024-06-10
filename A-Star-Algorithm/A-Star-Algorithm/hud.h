#pragma once
#include "tools.h"
#include "windowManager.h"
#include "map.h"

#define HUD_BLOCK_SIZE 60.f
#define HUD_BLOCK_ORIGIN 30.f
#define HUD_BLOCK_OUTLINE_THICKNESS 2.f
#define HUD_BLOCK_LENGTH 64
#define HUD_BLOCK_OFFSET 50.f

void initHud();
void updateHud(Window* _window);
void displayHud(Window* _window);
TileType getSelectedType();
