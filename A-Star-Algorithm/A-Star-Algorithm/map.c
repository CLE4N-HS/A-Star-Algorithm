#include "map.h"
#include "mouseManager.h"
#include "hud.h"
#include "customMath.h"

Block** map;
Block** savedMap;
int mapXSize;
int mapYSize;
sfVector2i startIndex;
sfVector2i finishIndex;
sfVector2i* openList;
sfVector2i* closedList;
int nbClosedListElements;
sfVector2i* searchList;
int nbNodes;
int nbOpenListElements;
sfVector2i currentNode = {0, 0};

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
	sfText_setFillColor(mapText, color(255, 255, 255));
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
	blockColor[TILE_CLOSED] = color(200, 50, 50);
	blockColor[TILE_CURRENT] = color(0, 127, 255);
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

			//sprintf(buffer, "%d", map[j][i].hCost);
			//sfText_setString(mapText, buffer);
			//sfText_setCharacterSize(mapText, 9);
			//sfText_setPosition(mapText, addVectorsf(map[j][i].pos, vector2f(BLOCK_ORIGIN / 2.f, -BLOCK_ORIGIN / 2.f)));
			//tmpTextBounds = sfText_getLocalBounds(mapText);
			//sfText_setOrigin(mapText, vector2f(tmpTextBounds.width / 2.f, tmpTextBounds.height));
			//sfRenderTexture_drawText(_window->renderTexture, mapText, NULL);

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


	sfRectangleShape_setFillColor(mapRectangle, blockColor[TILE_OPEN]);

	for (int i = 0; i < nbOpenListElements; i++)
	{
		sfRectangleShape_setPosition(mapRectangle, map[openList[i].y][openList[i].x].pos);
		sfRenderTexture_drawRectangleShape(_window->renderTexture, mapRectangle, NULL);

		//if (equalsVectors2i(openList[i], currentNode))
		//{
		//	sfRectangleShape_setFillColor(mapRectangle, blockColor[TILE_CURRENT]);
		//	sfRectangleShape_setPosition(mapRectangle, map[currentNode.y][currentNode.x].pos);
		//	sfRenderTexture_drawRectangleShape(_window->renderTexture, mapRectangle, NULL);

		//	sfRectangleShape_setFillColor(mapRectangle, blockColor[TILE_OPEN]);
		//}


		sprintf(buffer, "%d", map[openList[i].y][openList[i].x].gCost);
		sfText_setString(mapText, buffer);
		sfText_setCharacterSize(mapText, 10);
		sfText_setPosition(mapText, addVectorsf(map[openList[i].y][openList[i].x].pos, vector2f(-BLOCK_ORIGIN / 2.f, -BLOCK_ORIGIN / 2.f)));
		tmpTextBounds = sfText_getLocalBounds(mapText);
		sfText_setOrigin(mapText, vector2f(tmpTextBounds.width / 2.f, tmpTextBounds.height));
		sfRenderTexture_drawText(_window->renderTexture, mapText, NULL);

		sprintf(buffer, "%d", map[openList[i].y][openList[i].x].hCost);
		sfText_setString(mapText, buffer);
		sfText_setCharacterSize(mapText, 10);
		sfText_setPosition(mapText, addVectorsf(map[openList[i].y][openList[i].x].pos, vector2f(BLOCK_ORIGIN / 2.f, -BLOCK_ORIGIN / 2.f)));
		tmpTextBounds = sfText_getLocalBounds(mapText);
		sfText_setOrigin(mapText, vector2f(tmpTextBounds.width / 2.f, tmpTextBounds.height));
		sfRenderTexture_drawText(_window->renderTexture, mapText, NULL);


		sprintf(buffer, "%d", map[openList[i].y][openList[i].x].fCost);
		sfText_setString(mapText, buffer);
		sfText_setCharacterSize(mapText, 16);
		sfText_setPosition(mapText, addVectorsf(map[openList[i].y][openList[i].x].pos, vector2f(0.f, 10.f)));
		tmpTextBounds = sfText_getLocalBounds(mapText);
		sfText_setOrigin(mapText, vector2f(tmpTextBounds.width / 2.f, tmpTextBounds.height));
		sfRenderTexture_drawText(_window->renderTexture, mapText, NULL);

		sfVertexArray_clear(mapVertexArray);

		mapVertex.position = map[openList[i].y][openList[i].x].pos;
		sfVertexArray_append(mapVertexArray, mapVertex);
		mapVertex.position = vector2f(map[map[openList[i].y][openList[i].x].parent.y][map[openList[i].y][openList[i].x].parent.x].pos.x / 1.f, map[map[openList[i].y][openList[i].x].parent.y][map[openList[i].y][openList[i].x].parent.x].pos.y / 1.f);
		sfVertexArray_append(mapVertexArray, mapVertex);

		//sfRenderTexture_drawVertexArray(_window->renderTexture, mapVertexArray, NULL);
	}

	sfRectangleShape_setFillColor(mapRectangle, blockColor[TILE_CLOSED]);

	for (int i = 0; i < nbClosedListElements; i++)
	{
		sfRectangleShape_setPosition(mapRectangle, map[closedList[i].y][closedList[i].x].pos);
		sfRenderTexture_drawRectangleShape(_window->renderTexture, mapRectangle, NULL);

		//if (equalsVectors2i(closedList[i], currentNode))
		//{
		//	sfRectangleShape_setFillColor(mapRectangle, blockColor[TILE_CURRENT]);
		//	sfRectangleShape_setPosition(mapRectangle, map[currentNode.y][currentNode.x].pos);
		//	sfRenderTexture_drawRectangleShape(_window->renderTexture, mapRectangle, NULL);

		//	sfRectangleShape_setFillColor(mapRectangle, blockColor[TILE_OPEN]);
		//}

		sprintf(buffer, "%d", map[closedList[i].y][closedList[i].x].gCost);
		sfText_setString(mapText, buffer);
		sfText_setCharacterSize(mapText, 10);
		sfText_setPosition(mapText, addVectorsf(map[closedList[i].y][closedList[i].x].pos, vector2f(-BLOCK_ORIGIN / 2.f, -BLOCK_ORIGIN / 2.f)));
		tmpTextBounds = sfText_getLocalBounds(mapText);
		sfText_setOrigin(mapText, vector2f(tmpTextBounds.width / 2.f, tmpTextBounds.height));
		sfRenderTexture_drawText(_window->renderTexture, mapText, NULL);

		sprintf(buffer, "%d", map[closedList[i].y][closedList[i].x].hCost);
		sfText_setString(mapText, buffer);
		sfText_setCharacterSize(mapText, 10);
		sfText_setPosition(mapText, addVectorsf(map[closedList[i].y][closedList[i].x].pos, vector2f(BLOCK_ORIGIN / 2.f, -BLOCK_ORIGIN / 2.f)));
		tmpTextBounds = sfText_getLocalBounds(mapText);
		sfText_setOrigin(mapText, vector2f(tmpTextBounds.width / 2.f, tmpTextBounds.height));
		sfRenderTexture_drawText(_window->renderTexture, mapText, NULL);


		sprintf(buffer, "%d", map[closedList[i].y][closedList[i].x].fCost);
		sfText_setString(mapText, buffer);
		sfText_setCharacterSize(mapText, 16);
		sfText_setPosition(mapText, addVectorsf(map[closedList[i].y][closedList[i].x].pos, vector2f(0.f, 10.f)));
		tmpTextBounds = sfText_getLocalBounds(mapText);
		sfText_setOrigin(mapText, vector2f(tmpTextBounds.width / 2.f, tmpTextBounds.height));
		sfRenderTexture_drawText(_window->renderTexture, mapText, NULL);

		sfVertexArray_clear(mapVertexArray);

		mapVertex.position = map[closedList[i].y][closedList[i].x].pos;
		sfVertexArray_append(mapVertexArray, mapVertex);
		mapVertex.position = vector2f(map[map[closedList[i].y][closedList[i].x].parent.y][map[closedList[i].y][closedList[i].x].parent.x].pos.x / 1.f, map[map[closedList[i].y][closedList[i].x].parent.y][map[closedList[i].y][closedList[i].x].parent.x].pos.y / 1.f);
		sfVertexArray_append(mapVertexArray, mapVertex);

		//sfRenderTexture_drawVertexArray(_window->renderTexture, mapVertexArray, NULL);

	}




	if (nbNodes > 0) {
		sfVertexArray_clear(mapVertexArray);

		mapVertex.position = map[finishIndex.y][finishIndex.x].pos;
		sfVertexArray_append(mapVertexArray, mapVertex);
		mapVertex.position = map[searchList[0].y][searchList[0].x].pos;
		sfVertexArray_append(mapVertexArray, mapVertex);
		for (int i = 1; i < nbNodes; i++)
		{
			mapVertex.position = map[searchList[i].y][searchList[i].x].pos;
			sfVertexArray_append(mapVertexArray, mapVertex);
			mapVertex.position = map[searchList[i - 1].y][searchList[i - 1].x].pos;
			sfVertexArray_append(mapVertexArray, mapVertex);
		}
		mapVertex.position = map[searchList[nbNodes - 1].y][searchList[nbNodes - 1].x].pos;
		sfVertexArray_append(mapVertexArray, mapVertex);
		mapVertex.position = map[startIndex.y][startIndex.x].pos;
		sfVertexArray_append(mapVertexArray, mapVertex);

		sfRenderTexture_drawVertexArray(_window->renderTexture, mapVertexArray, NULL);
	}
}

void loadMap()
{
	mapXSize = 20;
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
		nbOpenListElements = 1;
		nbClosedListElements = 0;
		nbNodes = 0;
	}
	else {
		sfVector2i closestIndex = openList[0];
		int lowestFCost = map[closestIndex.y][closestIndex.x].fCost;
		sfVector2i* equalFCosts = NULL;
		int nbEqualFCosts = 0;
		int count = 0;
		int* counts = NULL;
		for (int i = 1; i < nbOpenListElements; i++)
		{
			if (map[openList[i].y][openList[i].x].fCost == lowestFCost) {
				nbEqualFCosts++;
				equalFCosts = realloc(equalFCosts, nbEqualFCosts * sizeof(sfVector2i));
				equalFCosts[nbEqualFCosts - 1] = openList[i];
				counts = realloc(counts, nbEqualFCosts * sizeof(int));
				counts[nbEqualFCosts - 1] = i;
			}
			else if (map[openList[i].y][openList[i].x].fCost < lowestFCost) {
				closestIndex = openList[i];
				lowestFCost = map[openList[i].y][openList[i].x].fCost;
				nbEqualFCosts = 0;
				count = i;
			}
		}

		if (nbEqualFCosts > 0) {
			closestIndex = equalFCosts[0];
			int lowestHCost = map[equalFCosts[0].y][equalFCosts[0].x].hCost;
			count = counts[0];
			for (int i = 1; i < nbEqualFCosts; i++)
			{
				if (map[equalFCosts[i].y][equalFCosts[i].x].hCost < lowestHCost) {
					lowestHCost = map[equalFCosts[i].y][equalFCosts[i].x].hCost;
					closestIndex = equalFCosts[i];
					count = counts[i];
				}
			}
		}

		removeElementInOpenList(count);
		addNodeInClosedList(closestIndex);
		

		printf("%d, %d\n", closestIndex.x, closestIndex.y);
		currentNode = closestIndex;
		//putParentInClosedList(closestIndex);
		if (equalsVectors2i(closestIndex, finishIndex)) {
			printf("done !");
			setSearchList();
		}

		free(equalFCosts);

		sfVector2i* possibleNeighbours = getPossibleNeighbours(closestIndex);

		int nbNeighbours = 0;
		sfVector2i* neighbours = getNeighbours(possibleNeighbours, &nbNeighbours);

		setNeighboursValues(neighbours, nbNeighbours, closestIndex);


		


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

	savedMap = map;
	openList = (sfVector2i*)calloc(1, sizeof(sfVector2i));
	openList[0] = startIndex;
	map[openList[0].y][openList[0].x].type = TILE_OPEN;
	map[openList[0].y][openList[0].x].gCost = 0;
	nbOpenListElements = 1;

	closedList = NULL;
	nbClosedListElements = 0;
	nbNodes = 0;
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
	mapXSize = 20;
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

	nbOpenListElements = 0;
}

sfBool isIndexInArray(sfVector2i _index)
{
	if (_index.x < 0 || _index.y < 0 || _index.x >= mapXSize || _index.y >= mapYSize)
		return sfFalse;

	return sfTrue;
}

sfVector2i* getPossibleNeighbours(sfVector2i _closestIndex)
{
	sfVector2i* possibleNeighbours = (sfVector2i*)calloc(8, sizeof(sfVector2i));
	possibleNeighbours[0] = vector2i(_closestIndex.x - 1, _closestIndex.y - 1); // Top Left
	possibleNeighbours[1] = vector2i(_closestIndex.x, _closestIndex.y - 1); // Top
	possibleNeighbours[2] = vector2i(_closestIndex.x + 1, _closestIndex.y - 1); // Top Right
	possibleNeighbours[3] = vector2i(_closestIndex.x - 1, _closestIndex.y); // Middle Left
	possibleNeighbours[4] = vector2i(_closestIndex.x + 1, _closestIndex.y); // Middle Right
	possibleNeighbours[5] = vector2i(_closestIndex.x - 1, _closestIndex.y + 1); // Bottom Left
	possibleNeighbours[6] = vector2i(_closestIndex.x, _closestIndex.y + 1); // Bottom
	possibleNeighbours[7] = vector2i(_closestIndex.x + 1, _closestIndex.y + 1); // Bottom Right

	return possibleNeighbours;
}

sfVector2i* getNeighbours(sfVector2i* _possibleNeighbours, int* _nbNeighbours)
{
	sfVector2i* neighbours = NULL;
	for (int path = 0; path < 8; path++)
	{
		if (isIndexInArray(_possibleNeighbours[path])) {
			if (!isNodeSolid(_possibleNeighbours[path]) && !isNodeInClosedList(_possibleNeighbours[path])) {
				*_nbNeighbours += 1;
				neighbours = (sfVector2i*)realloc(neighbours, *_nbNeighbours * sizeof(sfVector2i));
				neighbours[*_nbNeighbours - 1] = _possibleNeighbours[path];
			}
		}
	}
	return neighbours;
}

sfBool isNodeSolid(sfVector2i _node)
{
	if (map[_node.y][_node.x].type == TILE_WALL)
		return sfTrue;

	return sfFalse;
}

void addNodeInOpenList(sfVector2i _node)
{
	nbOpenListElements++;
	openList = (sfVector2i*)realloc(openList, nbOpenListElements * sizeof(sfVector2i));
	openList[nbOpenListElements - 1] = _node;
}

sfBool isNodeInOpenList(sfVector2i _node)
{
	for (int i = 0; i < nbOpenListElements; i++)
	{
		if (equalsVectors2i(_node, openList[i]))
			return sfTrue;
	}

	return sfFalse;
}

void setNeighboursValues(sfVector2i* _neighbours, int _nbNeighbours, sfVector2i _parent)
{
	for (int path = 0; path < _nbNeighbours; path++)
	{
		int distanceFromParent = getDistanceFromParent(_neighbours[path], _parent);

		int newPath = map[_parent.y][_parent.x].gCost + distanceFromParent;

		sfBool isShorter = (newPath < map[_neighbours[path].y][_neighbours[path].x].gCost ? sfTrue : sfFalse);
		sfBool isInOpenList = isNodeInOpenList(_neighbours[path]);

		if (isShorter || isInOpenList) {
			if (isShorter) {
				map[_neighbours[path].y][_neighbours[path].x].gCost = newPath;
				map[_neighbours[path].y][_neighbours[path].x].fCost = map[_neighbours[path].y][_neighbours[path].x].gCost + map[_neighbours[path].y][_neighbours[path].x].hCost;
				map[_neighbours[path].y][_neighbours[path].x].parent = _parent;
			}

			if (!isInOpenList)
				addNodeInOpenList(_neighbours[path]);
		}
	}
}

int getDistanceFromParent(sfVector2i _neighbour, sfVector2i _parent)
{
	sfVector2i diff = vector2i(_parent.x - _neighbour.x, _parent.y - _neighbour.y);
	if (abs(diff.x) == 1 && (abs(diff.y)) == 1)
		return DIAGONAL_COST;
	else
		return STRAIGHT_COST;
}

void addNodeInClosedList(sfVector2i _node)
{
	nbClosedListElements++;
	closedList = (sfVector2i*)realloc(closedList, nbClosedListElements * sizeof(sfVector2i));
	closedList[nbClosedListElements - 1] = _node;
}

sfBool isNodeInClosedList(sfVector2i _node)
{
	for (int i = 0; i < nbClosedListElements; i++)
	{
		if (equalsVectors2i(_node, closedList[i]))
			return sfTrue;
	}

	return sfFalse;
}

void setSearchList()
{
	sfVector2i parent = openList[nbOpenListElements - 1];
	nbNodes = 0;
	searchList = NULL;
	while (!equalsVectors2i(parent, startIndex))
	{
		nbNodes++;
		searchList = (sfVector2i*)realloc(searchList, nbNodes * sizeof(sfVector2i));
		searchList[nbNodes - 1] = parent;
		parent = map[parent.y][parent.x].parent;
	}
}

void removeElementInOpenList(int _count)
{
	for (int i = _count; i < nbOpenListElements - 1; i++)
	{
		openList[i] = openList[i + 1];
	}

	nbOpenListElements--;
	openList = (sfVector2i*)realloc(openList, nbOpenListElements * sizeof(openList));
}

void find()
{
	savedMap = map;
	openList = (sfVector2i*)calloc(1, sizeof(sfVector2i));
	openList[0] = startIndex;
	map[openList[0].y][openList[0].x].type = TILE_OPEN;
	map[openList[0].y][openList[0].x].gCost = 0;
	nbOpenListElements = 1;
	nbClosedListElements = 0;
	nbNodes = 0;

	while (1) {

		if (openList == NULL)
			return;

		sfVector2i closestIndex = openList[0];
		int lowestFCost = map[closestIndex.y][closestIndex.x].fCost;
		sfVector2i* equalFCosts = NULL;
		int nbEqualFCosts = 0;
		int count = 0;
		int* counts = NULL;
		for (int i = 1; i < nbOpenListElements; i++)
		{
			if (map[openList[i].y][openList[i].x].fCost == lowestFCost) {
				nbEqualFCosts++;
				equalFCosts = realloc(equalFCosts, nbEqualFCosts * sizeof(sfVector2i));
				equalFCosts[nbEqualFCosts - 1] = openList[i];
				counts = realloc(counts, nbEqualFCosts * sizeof(int));
				counts[nbEqualFCosts - 1] = i;
			}
			else if (map[openList[i].y][openList[i].x].fCost < lowestFCost) {
				closestIndex = openList[i];
				lowestFCost = map[openList[i].y][openList[i].x].fCost;
				nbEqualFCosts = 0;
				count = i;
			}
		}

		if (nbEqualFCosts > 0) {
			closestIndex = equalFCosts[0];
			int lowestHCost = map[equalFCosts[0].y][equalFCosts[0].x].hCost;
			count = counts[0];
			for (int i = 1; i < nbEqualFCosts; i++)
			{
				if (map[equalFCosts[i].y][equalFCosts[i].x].hCost < lowestHCost) {
					lowestHCost = map[equalFCosts[i].y][equalFCosts[i].x].hCost;
					closestIndex = equalFCosts[i];
					count = counts[i];
				}
			}
		}

		removeElementInOpenList(count);
		addNodeInClosedList(closestIndex);


		printf("%d, %d\n", closestIndex.x, closestIndex.y);
		currentNode = closestIndex;
		//putParentInClosedList(closestIndex);
		if (equalsVectors2i(closestIndex, finishIndex)) {
			printf("done !");
			setSearchList();
			return;
		}

		free(equalFCosts);

		sfVector2i* possibleNeighbours = getPossibleNeighbours(closestIndex);

		int nbNeighbours = 0;
		sfVector2i* neighbours = getNeighbours(possibleNeighbours, &nbNeighbours);

		setNeighboursValues(neighbours, nbNeighbours, closestIndex);





		free(possibleNeighbours);
		free(neighbours);
	}
}
