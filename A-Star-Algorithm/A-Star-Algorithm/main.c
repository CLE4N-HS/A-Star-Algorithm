#include "tools.h"
#include "windowManager.h"

int main(int _argc, char** _argv)
{
	Window* window = windowSetup();
	initWindow(window);

	while (!isDone(window))
	{
		updateWindow(window);
		displayWindow(window);
	}

	return 0;
}
