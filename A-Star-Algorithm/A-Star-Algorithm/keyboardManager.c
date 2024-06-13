//#include "mouseManager.h"
//
//typedef struct {
//	sfVector2f pos;
//	sfBool hasFocus;
//	sfBool hasReleased;
//	sfBool hasPressed;
//	sfMouseButton button;
//}Mouse;
//Mouse mouse;
//
//void initMouse()
//{
//	mouse.hasFocus = sfFalse;
//	mouse.hasReleased = sfTrue;
//	mouse.button = sfMouseLeft;
//	mouse.pos = NULLVECTOR2F;
//}
//
//void updateMouse(Window* _window)
//{
//	mouse.hasFocus = sfRenderWindow_hasFocus(_window->renderWindow);
//
//	if (!mouse.hasReleased)
//		mouse.hasReleased = !sfMouse_isButtonPressed(mouse.button);
//
//	mouse.pos = V2iToV2f(sfMouse_getPositionRenderWindow(_window->renderWindow));
//}
//
//sfBool leftClick()
//{
//	if (mouse.hasFocus && mouse.hasReleased && sfMouse_isButtonPressed(sfMouseLeft)) {
//		mouse.hasReleased = sfFalse;
//		mouse.button = sfMouseLeft;
//		return sfTrue;
//	}
//
//	return sfFalse;
//}
//
//sfBool leftClickUnreleased()
//{
//	if (mouse.hasFocus && sfMouse_isButtonPressed(sfMouseLeft)) {
//		//mouse.hasReleased = sfFalse;
//		mouse.button = sfMouseLeft;
//		return sfTrue;
//	}
//
//	return sfFalse;
//}
//
//sfVector2f getMousePos()
//{
//	return mouse.pos;
//}
