#pragma once
#include "tools.h"
#include "windowManager.h"
#include "map.h"

#define HUD_BLOCK_SIZE 60.f
#define HUD_BLOCK_ORIGIN 30.f
#define HUD_BLOCK_OUTLINE_THICKNESS 2.f
#define HUD_BLOCK_LENGTH 64
#define HUD_BLOCK_OFFSET 50.f

typedef enum {
	HUD_PATH = 0,
	HUD_WALL,
	HUD_START,
	HUD_FINISH,
	HUD_SEARCH,
	HUD_SAVE,
	HUD_RESET,
	HUD_DEFAULT,
	HUD_FIND,
	HUD_SHOW_OPEN_LIST,
	HUD_SHOW_CLOSED_LIST,
	HUD_SHOW_VALUES,
	HUD_SIZE_MINUS,
	HUD_SIZE_PLUS,
	HUD_MAZE,
	HUD_FRAMES,
	HUD_NB_MAX_TYPES
}HudTypes;

void initHud();
void updateHud(Window* _window);
void displayHud(Window* _window);
TileType getSelectedType();
void setMapFunctionType(HudTypes _type);
HudTypes getMapFunctionType();