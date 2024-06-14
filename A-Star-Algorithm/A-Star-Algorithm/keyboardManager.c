#include "keyboardManager.h"

typedef struct {
	sfBool hasFocus;
	sfBool hasReleased;
	sfBool hasPressed;
	sfKeyCode key;
}Keyboard;
Keyboard keyboard;

void initKeyboard()
{
	keyboard.hasFocus = sfFalse;
	keyboard.hasReleased = sfTrue;
	keyboard.hasPressed = sfFalse;
	keyboard.key = sfKeySpace;
}

void updateKeyboard(Window* _window)
{
	keyboard.hasFocus = sfRenderWindow_hasFocus(_window->renderWindow);

	if (!keyboard.hasReleased)
		keyboard.hasReleased = !sfKeyboard_isKeyPressed(keyboard.key);
}

sfBool hasPressed(sfKeyCode _key)
{
	if (keyboard.hasFocus && keyboard.hasReleased && sfKeyboard_isKeyPressed(_key)) {
		keyboard.hasReleased = sfFalse;
		keyboard.key = _key;
		return sfTrue;
	}

	return sfFalse;
}

sfBool isPressing(sfKeyCode _key)
{
	if (keyboard.hasFocus && sfKeyboard_isKeyPressed(_key))
		return sfTrue;

	return sfFalse;
}
