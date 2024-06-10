#include "map.h"
#include "mouseManager.h"
#include "hud.h"

Block** map;
Block** savedMap;
int mapXSize;
int mapYSize;
sfVector2i startIndex;
sfVector2i finishIndex;
sfVector2i* openList;
sfVector2i* closedList;
int nbIter;

sfRectangleShape* mapRectangle;
sfText* mapText;

sfColor blockColor[TILE_NB_MAX_TYPES];

void initMap()
{
	loadMap();

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
	blockColor[TILE_OPEN] = color(0, 0, 255);
	blockColor[TILE_CLOSED] = color(200, 0, 0);
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
			map[j][i].fCost = map[j][i].gCost + map[j][i].hCost;
		}
	}
}

void displayMap(Window* _window)
{
	char buffer[11];
	sfFloatRect tmpTextBounds;

	for (int j = 0; j < mapYSize; j++)
	{
		for (int i = 0; i < mapXSize; i++)
		{
			sfRectangleShape_setPosition(mapRectangle, map[j][i].pos);
			sfRectangleShape_setFillColor(mapRectangle, blockColor[map[j][i].type]);

			sfRenderTexture_drawRectangleShape(_window->renderTexture, mapRectangle, NULL);

			//sprintf(buffer, "%d", map[j][i].gCost);
			//sfText_setString(mapText, buffer);
			//sfText_setCharacterSize(mapText, 9);
			//sfText_setPosition(mapText, addVectorsf(map[j][i].pos, vector2f(-BLOCK_ORIGIN / 2.f, -BLOCK_ORIGIN / 2.f)));
			//tmpTextBounds = sfText_getLocalBounds(mapText);
			//sfText_setOrigin(mapText, vector2f(tmpTextBounds.width / 2.f, tmpTextBounds.height));
			//sfRenderTexture_drawText(_window->renderTexture, mapText, NULL);

			sprintf(buffer, "%d", map[j][i].hCost);
			sfText_setString(mapText, buffer);
			sfText_setCharacterSize(mapText, 9);
			sfText_setPosition(mapText, addVectorsf(map[j][i].pos, vector2f(BLOCK_ORIGIN / 2.f, -BLOCK_ORIGIN / 2.f)));
			tmpTextBounds = sfText_getLocalBounds(mapText);
			sfText_setOrigin(mapText, vector2f(tmpTextBounds.width / 2.f, tmpTextBounds.height));
			sfRenderTexture_drawText(_window->renderTexture, mapText, NULL);

			//sprintf(buffer, "%d", map[j][i].fCost);
			//sfText_setString(mapText, buffer);
			//sfText_setCharacterSize(mapText, 9);
			//sfText_setPosition(mapText, addVectorsf(map[j][i].pos, vector2f(0.f, BLOCK_ORIGIN / 2.f)));
			//tmpTextBounds = sfText_getLocalBounds(mapText);
			//sfText_setOrigin(mapText, vector2f(tmpTextBounds.width / 2.f, tmpTextBounds.height));
			//sfRenderTexture_drawText(_window->renderTexture, mapText, NULL);
		}
	}

	sfRectangleShape_setPosition(mapRectangle, map[startIndex.y][startIndex.x].pos);
	sfRectangleShape_setFillColor(mapRectangle, blockColor[TILE_START]);

	sfRenderTexture_drawRectangleShape(_window->renderTexture, mapRectangle, NULL);

	sfRectangleShape_setPosition(mapRectangle, map[finishIndex.y][finishIndex.x].pos);
	sfRectangleShape_setFillColor(mapRectangle, blockColor[TILE_FINISH]);

	sfRenderTexture_drawRectangleShape(_window->renderTexture, mapRectangle, NULL);
}

void loadMap()
{
	mapXSize = 14;
	mapYSize = 10;

	map = (Block**)calloc(mapYSize, sizeof(Block*));
	savedMap = (Block**)calloc(mapYSize, sizeof(Block*));

	for (int j = 0; j < mapYSize; j++)
	{
		map[j] = (Block*)calloc(mapXSize, sizeof(Block));
		savedMap[j] = (Block*)calloc(mapXSize, sizeof(Block));
	}

	defaultMap();
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

/// <summary>
/// 
/// currentNode -> get the lowest fCost in openList
/// if currentNode is equal to finish return complete path
/// (put currentNode in the closed list ?)
/// remove currentNode from openList
/// 
/// // maybe not
/// get all of his neighbours, put them in the openList
/// set their parent
/// set their gCost equal to the parent gCost + 10 or 14 if moved diagonally
/// //
/// 
/// </summary>

void search()
{
	if (!openList) {
		savedMap = map;
		openList = (sfVector2i*)calloc(1, sizeof(sfVector2i));
		openList[0] = startIndex;
		map[openList[0].y][openList[0].x].type = TILE_OPEN;
		map[openList[0].y][openList[0].x].gCost = 0;
		nbIter = 1;
	}
	else {
		sfVector2i closetIndex = NULLVECTOR2I;
		int lowestFCost = map[closetIndex.y][closetIndex.x].fCost;
		for (int i = 1; i < nbIter; i++)
		{
			if (map[openList[i].y][openList[i].x].fCost < lowestFCost) {
				closetIndex = openList[i];
				lowestFCost = map[openList[i].y][openList[i].x].fCost;
			}
		}
		sfVector2i* possibleNeighbours = (sfVector2i*)calloc(8, sizeof(sfVector2i));
		possibleNeighbours[0] = vector2i(closetIndex.x - 1, closetIndex.y - 1); // Top Left
		possibleNeighbours[1] = vector2i(closetIndex.x    , closetIndex.y - 1); // Top
		possibleNeighbours[2] = vector2i(closetIndex.x + 1, closetIndex.y - 1); // Top Right
		possibleNeighbours[3] = vector2i(closetIndex.x - 1, closetIndex.y    ); // Middle Left
		possibleNeighbours[4] = vector2i(closetIndex.x + 1, closetIndex.y    ); // Middle Right
		possibleNeighbours[5] = vector2i(closetIndex.x - 1, closetIndex.y + 1); // Bottom Left
		possibleNeighbours[6] = vector2i(closetIndex.x    , closetIndex.y + 1); // Bottom
		possibleNeighbours[7] = vector2i(closetIndex.x + 1, closetIndex.y + 1); // Bottom Right
		


		free(possibleNeighbours);

		nbIter++;
		openList = (sfVector2i*)realloc(openList, nbIter * sizeof(sfVector2i));
	}
}

void resetMap()
{
	for (int j = 0; j < mapYSize; j++)
	{
		for (int i = 0; i < mapXSize; i++)
		{
			map[j][i] = savedMap[j][i];

			if (map[j][i].type == TILE_START)
				startIndex = vector2i(i, j);
			if (map[j][i].type == TILE_FINISH)
				finishIndex = vector2i(i, j);
		}
	}
}

void saveMap()
{
	for (int j = 0; j < mapYSize; j++)
	{
		for (int i = 0; i < mapXSize; i++)
		{
			savedMap[j][i] = map[j][i];
		}
	}
}

void defaultMap()
{
	mapXSize = 14;
	mapYSize = 10;

	sfVector2f offsetPos = vector2f(WINDOW_LENGTH / 2.f - (float)mapXSize * BLOCK_LENGTH / 2.f + BLOCK_OUTLINE_THICKNESS + BLOCK_ORIGIN, WINDOW_HEIGHT / 1.75f - (float)mapYSize * BLOCK_LENGTH / 2.f + BLOCK_OUTLINE_THICKNESS + BLOCK_ORIGIN);

	for (int j = 0; j < mapYSize; j++)
	{
		for (int i = 0; i < mapXSize; i++)
		{
			map[j][i].type = TILE_PATH;
			map[j][i].index = vector2i(i, j);
			map[j][i].gCost = INT_MAX / 2;
			map[j][i].hCost = INT_MAX;
			map[j][i].fCost = INT_MAX;
			map[j][i].parent = NULLVECTOR2I;
			map[j][i].pos = addVectorsf(offsetPos, V2iToV2f(vector2i(BLOCK_LENGTH * i, BLOCK_LENGTH * j)));
			map[j][i].bounds = FloatRect(map[j][i].pos.x - BLOCK_ORIGIN, map[j][i].pos.y - BLOCK_ORIGIN, BLOCK_SIZE, BLOCK_SIZE);

			savedMap[j][i] = map[j][i];
		}
	}

	map[0][0].type = TILE_START;
	savedMap[0][0] = map[0][0];
	map[mapYSize - 1][mapXSize - 1].type = TILE_FINISH;
	savedMap[mapYSize - 1][mapXSize - 1] = map[mapYSize - 1][mapXSize - 1];
	startIndex = vector2i(0, 0);
	finishIndex = vector2i(mapXSize - 1, mapYSize - 1);

	nbIter = 0;
}

sfBool isIndexInArray(sfVector2i _index)
{
	if (_index.x < 0 || _index.y < 0 || _index.x >= mapXSize || _index.y >= mapYSize)
		return sfFalse;

	return sfTrue;
}
