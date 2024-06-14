#include "hud.h"
#include "customMath.h"
#include "mouseManager.h"

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
	HUD_NB_MAX_TYPES
}HudTypes;

sfVector2f hudBlockPos[HUD_NB_MAX_TYPES];
sfColor hudBlockColor[HUD_NB_MAX_TYPES];
char* hudTextString[HUD_NB_MAX_TYPES];
sfVector2f hudTextPos[HUD_NB_MAX_TYPES];
sfFloatRect hudBlockBounds[HUD_NB_MAX_TYPES];
void (*hudBlockFunctions[8]) = { search, saveMap, resetMap, defaultMap, find, toggleOpenList, toggleClosedList, toggleValues };

sfRectangleShape* hudRectangle;
sfText* hudText;

TileType selectedType;

void initHud()
{
	hudRectangle = sfRectangleShape_create();
	sfRectangleShape_setSize(hudRectangle, vector2f(HUD_BLOCK_SIZE, HUD_BLOCK_SIZE));
	sfRectangleShape_setOrigin(hudRectangle, vector2f(HUD_BLOCK_ORIGIN, HUD_BLOCK_ORIGIN));
	sfRectangleShape_setOutlineThickness(hudRectangle, HUD_BLOCK_OUTLINE_THICKNESS);
	sfRectangleShape_setOutlineColor(hudRectangle, color(128, 128, 128));

	hudText = sfText_create();
	sfText_setFillColor(hudText, color(255, 255, 255));
	sfText_setCharacterSize(hudText, 20);
	sfText_setFont(hudText, sfFont_createFromFile(FONT_PATH"GingerSoda.ttf"));

	hudTextString[HUD_PATH] = "Paths";
	hudTextString[HUD_WALL] = "Walls";
	hudTextString[HUD_START] = "Start";
	hudTextString[HUD_FINISH] = "Finish";
	hudTextString[HUD_SEARCH] = "Search";
	hudTextString[HUD_SAVE] = "Save";
	hudTextString[HUD_RESET] = "Reset";
	hudTextString[HUD_DEFAULT] = "Default";
	hudTextString[HUD_FIND] = "Find";
	hudTextString[HUD_SHOW_OPEN_LIST] = "OpenList";
	hudTextString[HUD_SHOW_CLOSED_LIST] = "ClosedList";
	hudTextString[HUD_SHOW_VALUES] = "Values";

	hudBlockColor[HUD_PATH] = color(255, 255, 255);
	hudBlockColor[HUD_WALL] = color(0, 0, 0);
	hudBlockColor[HUD_START] = color(0, 255, 0);
	hudBlockColor[HUD_FINISH] = color(255, 0, 0);
	hudBlockColor[HUD_SEARCH] = color(200, 200, 200);
	hudBlockColor[HUD_SAVE] = color(150, 150, 150);
	hudBlockColor[HUD_RESET] = color(100, 100, 100);
	hudBlockColor[HUD_DEFAULT] = color(50, 50, 50);
	hudBlockColor[HUD_FIND] = color(25, 25, 25);
	hudBlockColor[HUD_SHOW_OPEN_LIST] = color(0, 0, 255);
	hudBlockColor[HUD_SHOW_CLOSED_LIST] = color(200, 50, 50);
	hudBlockColor[HUD_SHOW_VALUES] = color(0, 255, 100);

	for (int i = 0; i < HUD_NB_MAX_TYPES; i++)
	{
		float tmpXPos = WINDOW_LENGTH / 2.f + (HUD_BLOCK_LENGTH + HUD_BLOCK_OFFSET) * i - ((HUD_BLOCK_LENGTH + HUD_BLOCK_OFFSET) * HUD_NB_MAX_TYPES / 2.f) + HUD_BLOCK_SIZE;
		hudBlockPos[i] = vector2f(tmpXPos, 100.f);
		hudBlockBounds[i] = FloatRect(hudBlockPos[i].x - HUD_BLOCK_ORIGIN, hudBlockPos[i].y - HUD_BLOCK_ORIGIN, HUD_BLOCK_SIZE, HUD_BLOCK_SIZE);

		hudTextPos[i] = vector2f(tmpXPos, 50.f);
	}

	selectedType = TILE_PATH;
}

void updateHud(Window* _window)
{
	sfVector2f mousePos = getMousePos();

	for (int i = 0; i < HUD_NB_MAX_TYPES; i++)
	{
		if (sfFloatRect_contains(&hudBlockBounds[i], mousePos.x, mousePos.y)) {
			if (leftClick()) {
				// doesnt work for some reasons
				//if (i >= HUD_SEARCH)
				//	hudBlockFunctions[i - HUD_SEARCH];
				//else
				//	selectedType = i;

				switch (i)
				{
				case HUD_SEARCH:           search();           break;
				case HUD_SAVE:             saveMap();          break;
				case HUD_RESET:            resetMap();         break;
				case HUD_DEFAULT:          defaultMap();       break;
				case HUD_FIND:             find();             break;
				case HUD_SHOW_OPEN_LIST:   toggleOpenList();   break;
				case HUD_SHOW_CLOSED_LIST: toggleClosedList(); break;
				case HUD_SHOW_VALUES:      toggleValues();     break;
				default:
					selectedType = i;
					break;
				}
			}
		}
	}
}

void displayHud(Window* _window)
{
	char buffer[30];

	for (int i = 0; i < HUD_NB_MAX_TYPES; i++)
	{
		sfText_setString(hudText, hudTextString[i]);
		sfText_setPosition(hudText, hudTextPos[i]);
		sfFloatRect tmpTextBounds = sfText_getLocalBounds(hudText);
		sfText_setOrigin(hudText, vector2f(tmpTextBounds.width / 2.f, tmpTextBounds.height));
		sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);

		sfRectangleShape_setPosition(hudRectangle, hudBlockPos[i]);
		sfRectangleShape_setFillColor(hudRectangle, hudBlockColor[i]);
		sfRenderTexture_drawRectangleShape(_window->renderTexture, hudRectangle, NULL);
	}

	sprintf(buffer, "FPS : %.0f", 1.f / getDeltaTime());
	sfText_setString(hudText, buffer);
	sfText_setPosition(hudText, vector2f(100.f, 100.f));
	sfRenderTexture_drawText(_window->renderTexture, hudText, NULL);
}

TileType getSelectedType()
{
	return selectedType;
}
