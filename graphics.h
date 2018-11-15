#ifndef GRAPHICS_SEEN
#define GRAPHICS_SEEN

#include "logic.h"
#include "beatmaps.h"
#include "gba.h"

#define NOTE_SIZE 20
#define HIT_Y 6
#define BACKGROUND_REDRAW_HEIGHT 5

// This function will be used to draw things that might have moved in a frame.
// For example, in a Snake game, draw the snake, the food, the score.
void drawAppState(AppState *state);

// If you have anything else you need accessible from outside the graphics.c
// file, you can add them here. You likely won't.

#endif
