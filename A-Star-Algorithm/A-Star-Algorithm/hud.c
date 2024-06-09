#include "hud.h"
#include "customMath.h"
#include "mouseManager.h"

sfVector2f hudBlockPos[HUD_NB_BLOCKS];
sfColor hudBlockColor[HUD_NB_BLOCKS];
char* hudTextString[HUD_NB_BLOCKS];
sfVector2f hudTextPos[HUD_NB_BLOCKS];
sfFloatRect hudBlockBounds[HUD_NB_BLOCKS];

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

	hudTextString[TILE_PATH] = "Paths";
	hudTextString[TILE_WALL] = "Walls";
	hudTextString[TILE_START] = "Start";
	hudTextString[TILE_FINISH] = "Finish";

	hudBlockColor[TILE_PATH] = color(255, 255, 255);
	hudBlockColor[TILE_WALL] = color(0, 0, 0);
	hudBlockColor[TILE_START] = color(0, 255, 0);
	hudBlockColor[TILE_FINISH] = color(255, 0, 0);

	for (int i = 0; i < HUD_NB_BLOCKS; i++)
	{
		float tmpXPos = WINDOW_LENGTH / 2.f + (HUD_BLOCK_LENGTH + HUD_BLOCK_OFFSET) * i - ((HUD_BLOCK_LENGTH + HUD_BLOCK_OFFSET) * TILE_NB_MAX_TYPES / 2.f) + HUD_BLOCK_SIZE;
		hudBlockPos[i] = vector2f(tmpXPos, 100.f);
		hudBlockBounds[i] = FloatRect(hudBlockPos[i].x - HUD_BLOCK_ORIGIN, hudBlockPos[i].y - HUD_BLOCK_ORIGIN, HUD_BLOCK_SIZE, HUD_BLOCK_SIZE);

		hudTextPos[i] = vector2f(tmpXPos, 50.f);
	}

	selectedType = TILE_PATH;
}

void updateHud(Window* _window)
{
	sfVector2f mousePos = getMousePos();

	for (int i = 0; i < HUD_NB_BLOCKS; i++)
	{
		if (sfFloatRect_contains(&hudBlockBounds[i], mousePos.x, mousePos.y)) {
			if (leftClick())
				selectedType = i;
		}
	}
}

void displayHud(Window* _window)
{
	for (int i = 0; i < HUD_NB_BLOCKS; i++)
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
}

TileType getSelectedType()
{
	return selectedType;
}
