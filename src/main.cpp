#include "config.h"

#include <cstdio>
#include <string>

#include <SDL.h>

#include "envelope.h"
#include "fs_utils.h"
#include "libs/nuked/opl3.h" // C library test
#include "logging.h"
#include "midi.h"

int main()
{
	printf("program version: %s\n", VERSION);

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

	// code in misc test
	const std::string home = to_native_path("~");

	// code in midi test
	// MIDI_ListAll(nullptr);
	
	// code in hardware test
	Envelope("foobar");

	// code in gui test
	LOG_MSG("Bye!\n");
	return 0;
}
