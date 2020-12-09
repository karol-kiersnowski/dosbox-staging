#include "config.h"

#include <cstdio>

#include <SDL.h>

#include "libs/nuked/opl3.h" // C library test

int main()
{
	printf("program version: %s\n", VERSION_STR);

	// SDL2 test
	static_assert(SDL_VERSION_ATLEAST(2, 0, 2));

	SDL_version compiled;
	SDL_VERSION(&compiled);
	printf("SDL (compiled):  %d.%d.%d\n", compiled.major, compiled.minor,
	       compiled.patch);

	SDL_version linked;
	SDL_GetVersion(&linked);
	printf("SDL (linked):    %d.%d.%d\n", linked.major, linked.minor,
	       linked.patch);

	// C library test
	opl3_chip chip = {};
	OPL3_Reset(&chip, 0);

	return 0;
}
