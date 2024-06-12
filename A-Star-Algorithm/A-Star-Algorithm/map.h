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
	TILE_CURRENT,
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
sfVector2i* getPossibleNeighbours(sfVector2i _closestIndex);
sfVector2i* getNeighbours(sfVector2i* _possibleNeighbours, int* _nbNeighbours);
sfBool isNodeSolid(sfVector2i _node);
void addNodeInOpenList(sfVector2i _node);
sfBool isNodeInOpenList(sfVector2i _node);
void setNeighboursValues(sfVector2i* _neighbours, int _nbNeighbours, sfVector2i _parent);
int getDistanceFromParent(sfVector2i _neighbour, sfVector2i _parent);
void addNodeInClosedList(sfVector2i _node);
sfBool isNodeInClosedList(sfVector2i _node);
void setSearchList();
void removeElementInOpenList(int _count);
