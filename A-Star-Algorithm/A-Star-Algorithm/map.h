#pragma once
#include "tools.h"
#include "windowManager.h"

#define BLOCK_SIZE 30.f
#define BLOCK_ORIGIN 15.f
#define BLOCK_OUTLINE_THICKNESS 1.f
#define BLOCK_LENGTH 32

typedef enum {
	TILE_PATH,
	TILE_WALL,
	TILE_START,
	TILE_FINISH,
	TILE_NB_MAX_TYPES
}TileTypes;

typedef struct {
	TileTypes type;
	sfVector2i index;
	int gCost;
	int hCost;
	int fCost;
	sfVector2i parent;
}Block;

void initMap();
void updateMap(Window* _window);
void displayMap(Window* _window);
