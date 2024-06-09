#pragma once
#include "tools.h"
#include "windowManager.h"

#define BLOCK_SIZE 40.f
#define BLOCK_ORIGIN 20.f
#define BLOCK_OUTLINE_THICKNESS 1.f
#define BLOCK_LENGTH 42

#define STRAIGHT_COST 10
#define DIAGONAL_COST 14

typedef enum {
	TILE_PATH = 0,
	TILE_WALL,
	TILE_START,
	TILE_FINISH,
	TILE_NB_MAX_TYPES
}TileType;

typedef struct {
	TileType type;
	sfVector2i index;
	int gCost;
	int hCost;
	int fCost;
	sfVector2i parent;
	sfVector2f pos;
	sfFloatRect bounds;
}Block;

void initMap();
void updateMap(Window* _window);
void displayMap(Window* _window);

void removeType(TileType _type);

