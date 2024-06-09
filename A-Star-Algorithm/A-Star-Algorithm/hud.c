#include "hud.h"
#include "map.h"
#include "customMath.h"

sfVector2f hudBlockPos[TILE_NB_MAX_TYPES];
sfColor hudBlockColor[TILE_NB_MAX_TYPES];
char* hudTextString[TILE_NB_MAX_TYPES];
sfVector2f hudTextPos[TILE_NB_MAX_TYPES];

sfRectangleShape* hudRectangle;
sfText* hudText;

void initHud()
{
	hudRectangle = sfRectangleShape_create();
	sfRectangleShape_setSize(hudRectangle, vector2f(HUD_BLOCK_SIZE, HUD_BLOCK_SIZE));
	sfRectangleShape_setOrigin(hudRectangle, vector2f(HUD_BLOCK_ORIGIN, HUD_BLOCK_ORIGIN));
	sfRectangleShape_setOutlineThickness(hudRectangle, HUD_BLOCK_OUTLINE_THICKNESS);
	sfRectangleShape_setOutlineColor(hudRectangle, color(128, 128, 128));

	hudText = sfText_create();

	hudTextString[TILE_PATH] = "Paths";
	hudTextString[TILE_WALL] = "Walls";
	hudTextString[TILE_START] = "Start";
	hudTextString[TILE_FINISH] = "Finish";

	for (int i = 0; i < TILE_NB_MAX_TYPES; i++)
	{
		hudBlockPos[i] = vector2f(WINDOW_LENGTH / 2.f - TILE_NB_MAX_TYPES / 2.f * i, 100.f);
		hudBlockPos[i] = vector2f(lerp(0.f, WINDOW_LENGTH, 1.f / lerp(0.f, TILE_NB_MAX_TYPES - 1, i)), 100.f);
		hudBlockPos[i] = vector2f( 1.f / lerp(0.f, TILE_NB_MAX_TYPES - 1, i) * WINDOW_LENGTH, 100.f);
		hudBlockColor[i] = getTileColor(i);

		hudTextPos[i] = vector2f(WINDOW_LENGTH / 2.f - TILE_NB_MAX_TYPES / 2.f * i, 50.f);
	}
}

void updateHud(Window* _window)
{

}

void displayHud(Window* _window)
{
	for (int i = 0; i < TILE_NB_MAX_TYPES; i++)
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
