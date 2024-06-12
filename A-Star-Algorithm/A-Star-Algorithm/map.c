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
int nbNodeClosedList;
sfVector2i* searchList;
int nbNodes;
int nbIter;

sfRectangleShape* mapRectangle;
sfText* mapText;
sfVertexArray* mapVertexArray;
sfVertex mapVertex;

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

	mapVertexArray = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(mapVertexArray, sfLines);
	mapVertex.color = color(0, 255, 100);

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


	if (openList != NULL) {
		//setSearchList();
		sfVertexArray_clear(mapVertexArray);
		for (int i = 0; i < nbNodes; i++)
		{
			mapVertex.position = map[searchList[i].y][searchList[i].x].pos;
			sfVertexArray_append(mapVertexArray, mapVertex);
		}
		sfRenderTexture_drawVertexArray(_window->renderTexture, mapVertexArray, NULL);
		free(searchList);
	}
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
		nbNodeClosedList = 0;
		nbNodes = 0;
	}
	else {
		sfVector2i closestIndex = startIndex;
		int lowestFCost = map[closestIndex.y][closestIndex.x].fCost;
		sfVector2i* equalFCosts = NULL;
		int nbEqualFCosts = 0;
		for (int i = 1; i < nbIter; i++)
		{
			if (!isNodeInClosedList(openList[i])) {
				if (map[openList[i].y][openList[i].x].fCost == lowestFCost) {
					nbEqualFCosts++;
					equalFCosts = realloc(equalFCosts, nbEqualFCosts * sizeof(sfVector2i));
					equalFCosts[nbEqualFCosts - 1] = openList[i];
				}
				else if (map[openList[i].y][openList[i].x].fCost < lowestFCost) {
					closestIndex = openList[i];
					lowestFCost = map[openList[i].y][openList[i].x].fCost;
					nbEqualFCosts = 0;
				}
			}
		}

		if (nbEqualFCosts > 0) {
			closestIndex = equalFCosts[0];
			int lowestHCost = map[equalFCosts[0].y][equalFCosts[0].x].hCost;
			for (int i = 1; i < nbEqualFCosts; i++)
			{
				if (map[equalFCosts[i].y][equalFCosts[i].x].hCost < lowestHCost) {
					lowestHCost = map[equalFCosts[i].y][equalFCosts[i].x].hCost;
					closestIndex = equalFCosts[i];
				}
			}
		}

		printf("%d, %d\n", closestIndex.x, closestIndex.y);
		putNodeInClosedList(closestIndex);
		//putParentInClosedList(closestIndex);
		if (equalsVectors2i(closestIndex, finishIndex)) {
			printf("done !");
			setSearchList();
		}

		free(equalFCosts);

		sfVector2i* possibleNeighbours = (sfVector2i*)calloc(8, sizeof(sfVector2i));
		setPossibleNeighbours(possibleNeighbours, closestIndex);

		int nbPath = 0;
		sfVector2i* neighbours = (sfVector2i*)calloc(8, sizeof(sfVector2i));
		setNeighbours(neighbours, possibleNeighbours, &nbPath);

		setNeighboursValues(neighbours, nbPath, closestIndex);


		


		free(possibleNeighbours);
		free(neighbours);
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

void setPossibleNeighbours(sfVector2i* _possibleNeighbours, sfVector2i _closestIndex)
{
	_possibleNeighbours[0] = vector2i(_closestIndex.x - 1, _closestIndex.y - 1); // Top Left
	_possibleNeighbours[1] = vector2i(_closestIndex.x, _closestIndex.y - 1); // Top
	_possibleNeighbours[2] = vector2i(_closestIndex.x + 1, _closestIndex.y - 1); // Top Right
	_possibleNeighbours[3] = vector2i(_closestIndex.x - 1, _closestIndex.y); // Middle Left
	_possibleNeighbours[4] = vector2i(_closestIndex.x + 1, _closestIndex.y); // Middle Right
	_possibleNeighbours[5] = vector2i(_closestIndex.x - 1, _closestIndex.y + 1); // Bottom Left
	_possibleNeighbours[6] = vector2i(_closestIndex.x, _closestIndex.y + 1); // Bottom
	_possibleNeighbours[7] = vector2i(_closestIndex.x + 1, _closestIndex.y + 1); // Bottom Right
}

void setNeighbours(sfVector2i* _neighbours, sfVector2i* _possibleNeighbours, int* _nbPath)
{
	for (int path = 0; path < 8; path++)
	{
		if (isIndexInArray(_possibleNeighbours[path])) {
			if (!isNodeSolid(_possibleNeighbours[path])) {
				_neighbours[*_nbPath] = _possibleNeighbours[path];
				addNodeInOpenList(_neighbours[*_nbPath]);
				*_nbPath += 1;
			}
		}
	}
}

sfBool isNodeSolid(sfVector2i _node)
{
	if (map[_node.y][_node.x].type == TILE_WALL)
		return sfTrue;

	return sfFalse;
}

void addNodeInOpenList(sfVector2i _node)
{
	nbIter++;
	openList = (sfVector2i*)realloc(openList, nbIter * sizeof(sfVector2i));
	openList[nbIter - 1] = _node;
}

void setNeighboursValues(sfVector2i* _neighbours, int _nbPath, sfVector2i _parent)
{
	for (int path = 0; path < _nbPath; path++)
	{
		map[_neighbours[path].y][_neighbours[path].x].parent = _parent;

		int distanceFromParent = 0;
		if (abs(_neighbours[path].x) == 1 && abs(_neighbours[path].y) == 1)
			distanceFromParent = DIAGONAL_COST;
		else
			distanceFromParent = STRAIGHT_COST;

		map[_neighbours[path].y][_neighbours[path].x].gCost = map[_parent.y][_parent.x].gCost + distanceFromParent;
		map[_neighbours[path].y][_neighbours[path].x].fCost = map[_neighbours[path].y][_neighbours[path].x].gCost + map[_neighbours[path].y][_neighbours[path].x].hCost;
	}
}

void putNodeInClosedList(sfVector2i _node)
{
	nbNodeClosedList++;
	closedList = (sfVector2i*)realloc(closedList, nbNodeClosedList * sizeof(sfVector2i));
	closedList[nbNodeClosedList - 1] = _node;
}

void putParentInClosedList(sfVector2i _node)
{
	nbNodeClosedList++;
	closedList = (sfVector2i*)realloc(closedList, nbNodeClosedList * sizeof(sfVector2i));
	closedList[nbNodeClosedList - 1] = map[_node.y][_node.x].parent;
}

sfBool isNodeInClosedList(sfVector2i _node)
{
	for (int i = 0; i < nbNodeClosedList; i++)
	{
		if (equalsVectors2i(_node, closedList[i]))
			return sfTrue;
	}

	return sfFalse;
}

void setSearchList()
{
	sfVector2i parent = openList[nbIter - 1];
	nbNodes = 0;
	searchList = NULL;
	//while (!equalsVectors2i(parent, startIndex))
	//{
	//	nbNodes++;
	//	searchList = (sfVector2i*)realloc(searchList, nbNodes * sizeof(sfVector2i));
	//	searchList[nbNodes - 1] = parent;
	//	parent = map[parent.y][parent.x].parent;
	//}
}
