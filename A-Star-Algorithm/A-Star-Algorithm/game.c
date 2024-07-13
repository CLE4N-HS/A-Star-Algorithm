#include "game.h"
#include "map.h"
#include "hud.h"
#include "mouseManager.h"
#include "keyboardManager.h"

sfThread* gameThread;
sfThread* fpsThread;

sfText* gameText;
char fpsBuffer[10];
char fpsBufferThread2[10];

void initGame(Window* _window)
{
	gameMutex = sfMutex_create();
	gameThread = sfThread_create(updateGameThread, _window);

	fpsMutex = sfMutex_create();
	fpsThread = sfThread_create(updateFpsThread, _window);

	gameText = sfText_create();
	sfText_setFont(gameText, sfFont_createFromFile(FONT_PATH"GingerSoda.ttf"));
	sfText_setFillColor(gameText, color(255, 255, 255));
	sfText_setCharacterSize(gameText, 20);
	sfText_setOutlineColor(gameText, color(0, 0, 0));
	sfText_setOutlineThickness(gameText, 2.f);
	sprintf(fpsBuffer, "FPS : %.0f", 0.f);
	sprintf(fpsBufferThread2, "FPS : %.0f", 0.f);

	initMap();
	initHud();
	initMouse();
	initKeyboard();

	sfThread_launch(gameThread);
	sfThread_launch(fpsThread);
}

void updateGame(Window* _window)
{
	updateMouse(_window);
	updateKeyboard(_window);
	//updateMap(_window);
	updateHud(_window);
	updateFPS();
}

void updateGameThread(Window* _window)
{
	while (1) // peak programming loop
	{
		restartClockThread2();

		updateMap(_window);
		updateFPSThread2();
	}
}

void updateFpsThread(Window* _window)
{
	while (1) // peak programming loop again
	{
		HudTypes tmpType = getMapFunctionType();

		if (tmpType == HUD_FRAMES)
		{
			changeFrames();
			setMapFunctionType(HUD_NB_MAX_TYPES);
		}
	}
}

void displayGame(Window* _window)
{
	displayMap(_window);
	displayHud(_window);
	displayFPS(_window);
}

void updateFPS()
{
	static float fpsTimer = 1.f;
	fpsTimer += getDeltaTime();
	if (fpsTimer > 0.2f) {
		sprintf(fpsBuffer, "FPS : %.0f", 1.f / getDeltaTime());
		fpsTimer = 0.f;
	}
}

void updateFPSThread2()
{
	static float fpsTimerThread2 = 1.f;
	fpsTimerThread2 += getDeltaTimeThread2();
	if (fpsTimerThread2 > 0.2f) {
		sprintf(fpsBufferThread2, "FPS THREAD 2 : %.0f", 1.f / getDeltaTimeThread2());
		fpsTimerThread2 = 0.f;
	}
}

void displayFPS(Window* _window)
{
	return;
	sfText_setString(gameText, fpsBuffer);
	sfText_setPosition(gameText, vector2f(30.f, 30.f));
	sfRenderTexture_drawText(_window->renderTexture, gameText, NULL);

	sfText_setString(gameText, fpsBufferThread2);
	sfText_setPosition(gameText, vector2f(30.f, 60.f));
	sfRenderTexture_drawText(_window->renderTexture, gameText, NULL);
}
