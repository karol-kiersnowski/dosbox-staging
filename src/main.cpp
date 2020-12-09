#include "config.h"

#include <cstdio>

int main()
{
	constexpr auto version = VERSION_STR;
	printf("version: %s\n", version);
	return 0;
}
