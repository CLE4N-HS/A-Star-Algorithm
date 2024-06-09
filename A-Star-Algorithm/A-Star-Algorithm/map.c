#include "map.h"

Block** map;
int mapXSize;
int mapYSize;

sfRectangleShape* mapRectangle;
sfColor blockColor[TILE_NB_MAX_TYPES];

void initMap()
{
	mapXSize = 30;
	mapYSize = 20;

	map = (Block**)calloc(mapYSize, sizeof(Block*));

	for (int j = 0; j < mapYSize; j++)
	{
		for (int i = 0; i < mapXSize; i++)
		{
			map[j] = (Block*)calloc(mapXSize, sizeof(Block));

			map[j][i].type = TILE_PATH;
			map[j][i].index = vector2i(i, j);
			map[j][i].gCost = INT_MAX;
			map[j][i].hCost = INT_MAX;
			map[j][i].fCost = INT_MAX;
			map[j][i].parent = NULLVECTOR2I;
		}
	}

	mapRectangle = sfRectangleShape_create();
	sfRectangleShape_setSize(mapRectangle, vector2f(BLOCK_SIZE, BLOCK_SIZE));
	sfRectangleShape_setOrigin(mapRectangle, vector2f(BLOCK_ORIGIN, BLOCK_ORIGIN));
	sfRectangleShape_setOutlineThickness(mapRectangle, BLOCK_OUTLINE_THICKNESS);
	sfRectangleShape_setOutlineColor(mapRectangle, color(128, 128, 128));

	blockColor[TILE_PATH] = color(255, 255, 255);
	blockColor[TILE_WALL] = color(0, 0, 0);
	blockColor[TILE_START] = color(0, 255, 0);
	blockColor[TILE_FINISH] = color(255, 0, 0);
}

void updateMap(Window* _window)
{

}

void displayMap(Window* _window)
{
	sfVector2f offsetPos = vector2f(WINDOW_LENGTH / 2.f - (float)mapXSize * BLOCK_LENGTH / 2.f + BLOCK_OUTLINE_THICKNESS + BLOCK_ORIGIN, WINDOW_HEIGHT / 1.75f - (float)mapYSize * BLOCK_LENGTH / 2.f + BLOCK_OUTLINE_THICKNESS + BLOCK_ORIGIN);

	for (int j = 0; j < mapYSize; j++)
	{
		for (int i = 0; i < mapXSize; i++)
		{
			sfRectangleShape_setPosition(mapRectangle, addVectorsf(offsetPos, V2iToV2f(vector2i(BLOCK_LENGTH * i, BLOCK_LENGTH * j))));
			sfRectangleShape_setFillColor(mapRectangle, blockColor[map[j][i].type]);

			sfRenderTexture_drawRectangleShape(_window->renderTexture, mapRectangle, NULL);
		}
	}
}

sfColor getTileColor(TileTypes _type)
{
	return blockColor[_type];
}
