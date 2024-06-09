#include "map.h"
#include "mouseManager.h"
#include "hud.h"

Block** map;
int mapXSize;
int mapYSize;
sfVector2i startIndex;
sfVector2i finishIndex;

sfRectangleShape* mapRectangle;
sfText* mapText;

sfColor blockColor[TILE_NB_MAX_TYPES];

void initMap()
{
	mapXSize = 20;
	mapYSize = 20;

	map = (Block**)calloc(mapYSize, sizeof(Block*));

	sfVector2f offsetPos = vector2f(WINDOW_LENGTH / 2.f - (float)mapXSize * BLOCK_LENGTH / 2.f + BLOCK_OUTLINE_THICKNESS + BLOCK_ORIGIN, WINDOW_HEIGHT / 1.75f - (float)mapYSize * BLOCK_LENGTH / 2.f + BLOCK_OUTLINE_THICKNESS + BLOCK_ORIGIN);

	for (int j = 0; j < mapYSize; j++)
	{
		map[j] = (Block*)calloc(mapXSize, sizeof(Block));
		
		for (int i = 0; i < mapXSize; i++)
		{
			map[j][i].type = TILE_PATH;
			map[j][i].index = vector2i(i, j);
			map[j][i].gCost = INT_MAX;
			map[j][i].hCost = INT_MAX;
			map[j][i].fCost = INT_MAX;
			map[j][i].parent = NULLVECTOR2I;
			map[j][i].pos = addVectorsf(offsetPos, V2iToV2f(vector2i(BLOCK_LENGTH * i, BLOCK_LENGTH * j)));
			map[j][i].bounds = FloatRect(map[j][i].pos.x - BLOCK_ORIGIN, map[j][i].pos.y - BLOCK_ORIGIN, BLOCK_SIZE, BLOCK_SIZE);
		}
	}

	map[0][0].type = TILE_START;
	map[mapYSize - 1][mapXSize - 1].type = TILE_FINISH;
	startIndex = vector2i(0, 0);
	finishIndex = vector2i(mapXSize - 1, mapYSize - 1);

	mapRectangle = sfRectangleShape_create();
	sfRectangleShape_setSize(mapRectangle, vector2f(BLOCK_SIZE, BLOCK_SIZE));
	sfRectangleShape_setOrigin(mapRectangle, vector2f(BLOCK_ORIGIN, BLOCK_ORIGIN));
	sfRectangleShape_setOutlineThickness(mapRectangle, BLOCK_OUTLINE_THICKNESS);
	sfRectangleShape_setOutlineColor(mapRectangle, color(128, 128, 128));

	mapText = sfText_create();
	sfText_setFillColor(mapText, color(50, 50, 50));
	sfText_setCharacterSize(mapText, 10);
	sfText_setFont(mapText, sfFont_createFromFile(FONT_PATH"Square.ttf"));

	blockColor[TILE_PATH] = color(255, 255, 255);
	blockColor[TILE_WALL] = color(0, 0, 0);
	blockColor[TILE_START] = color(0, 255, 0);
	blockColor[TILE_FINISH] = color(255, 0, 0);
}

void updateMap(Window* _window)
{
	sfVector2f mousePos = getMousePos();
	sfBool hasLeftCLick = leftClickUnreleased();

	for (int j = 0; j < mapYSize; j++)
	{
		for (int i = 0; i < mapXSize; i++)
		{
			if (hasLeftCLick && map[j][i].type != TILE_START && map[j][i].type != TILE_FINISH && sfFloatRect_contains(&map[j][i].bounds, mousePos.x, mousePos.y)) {
				TileType selectedType = getSelectedType();
				if (selectedType == TILE_START || selectedType == TILE_FINISH)
					removeType(selectedType);

				map[j][i].type = selectedType;

				if (map[j][i].type == TILE_START)
					startIndex = vector2i(i, j);
				else if (map[j][i].type == TILE_FINISH)
					finishIndex = vector2i(i, j);
			}

			map[j][i].hCost = getHCost(vector2i(i, j));
		}
	}
}

void displayMap(Window* _window)
{
	sfVector2f offsetPos = vector2f(WINDOW_LENGTH / 2.f - (float)mapXSize * BLOCK_LENGTH / 2.f + BLOCK_OUTLINE_THICKNESS + BLOCK_ORIGIN, WINDOW_HEIGHT / 1.75f - (float)mapYSize * BLOCK_LENGTH / 2.f + BLOCK_OUTLINE_THICKNESS + BLOCK_ORIGIN);
	char buffer[11];

	for (int j = 0; j < mapYSize; j++)
	{
		for (int i = 0; i < mapXSize; i++)
		{
			sfRectangleShape_setPosition(mapRectangle, map[j][i].pos);
			sfRectangleShape_setFillColor(mapRectangle, blockColor[map[j][i].type]);

			sfRenderTexture_drawRectangleShape(_window->renderTexture, mapRectangle, NULL);

			sprintf(buffer, "%d", map[j][i].hCost);
			sfText_setString(mapText, buffer);
			sfText_setCharacterSize(mapText, 8);
			sfText_setPosition(mapText, addVectorsf(map[j][i].pos, vector2f(BLOCK_ORIGIN / 2.f, -BLOCK_ORIGIN / 2.f)));
			sfFloatRect tmpTextBounds = sfText_getLocalBounds(mapText);
			sfText_setOrigin(mapText, vector2f(tmpTextBounds.width / 2.f, tmpTextBounds.height));
			sfRenderTexture_drawText(_window->renderTexture, mapText, NULL);
		}
	}
}

void removeType(TileType _type)
{
	for (int j = 0; j < mapYSize; j++)
	{
		for (int i = 0; i < mapXSize; i++)
		{
			if (map[j][i].type == _type)
				map[j][i].type = TILE_PATH;
		}
	}
}

int getHCost(sfVector2i _index)
{
	int xLength = abs(_index.x - finishIndex.x);
	int yLength = abs(_index.y - finishIndex.y);
	int diff = min(xLength, yLength);
	return diff * DIAGONAL_COST + (xLength - diff) * STRAIGHT_COST + (yLength - diff) * STRAIGHT_COST;
}
