
#include "beatmaps.h"

// A beatmap is defined as a unsigned short (16 bits), where the bits represent which beats should be hit 
// Looks like this, where Xs represent unused bits
// xxxx asdf xxx(space bar) jkl;

const unsigned short map1[6] =
{
	0x0100,
	0x0008,
	0x0204,
	0x0010,
	0x0801,
	0x0F1F
};