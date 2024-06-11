#pragma once
#include "tools.h"
#include "windowManager.h"

#define BLOCK_SIZE 60.f
#define BLOCK_ORIGIN 30.f
#define BLOCK_OUTLINE_THICKNESS 1.f
#define BLOCK_LENGTH 62

#define STRAIGHT_COST 10
#define DIAGONAL_COST 14

typedef enum {
	TILE_PATH = 0,
	TILE_WALL,
	TILE_START,
	TILE_FINISH,
	TILE_OPEN,
	TILE_CLOSED,
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

void loadMap();
void removeType(TileType _type);
int getHCost(sfVector2i _index);
void saveMap();
void search();
void resetMap();
void defaultMap();
sfBool isIndexInArray(sfVector2i _index);
void setPossibleNeighbours(sfVector2i* _possibleNeighbours, sfVector2i _closestIndex);
void setNeighbours(sfVector2i* _neighbours, sfVector2i* _possibleNeighbours, int* _nbPath);
sfBool isNodeSolid(sfVector2i _node);
void addNodeInOpenList(sfVector2i _node);
void setNeighboursValues(sfVector2i* _neighbours, int _nbPath, sfVector2i _parent);
