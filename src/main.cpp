#include "config.h"

#include <cstdio>

#include <SDL.h>

int main()
{
	printf("program version: %s\n", VERSION_STR);

	static_assert(SDL_VERSION_ATLEAST(2, 0, 2));

	SDL_version compiled;
	SDL_VERSION(&compiled);
	printf("SDL (compiled):  %d.%d.%d\n", compiled.major, compiled.minor,
	       compiled.patch);

	SDL_version linked;
	SDL_GetVersion(&linked);
	printf("SDL (linked):    %d.%d.%d\n", linked.major, linked.minor,
	       linked.patch);

	return 0;
}
