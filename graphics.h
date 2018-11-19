#ifndef GRAPHICS_SEEN
#define GRAPHICS_SEEN

#include "logic.h"
#include "beatmaps.h"
#include "gba.h"

#define NOTE_SIZE 20
#define NOTE_PADDING 10
//Oh god i'm so sorry. HIT_Y_ACTUAL is where the hit is registered. it's purely graphical, so you could put it anywhere, which is nifty imo.
#define HIT_Y 2
#define HIT_Y_MULT 60
#define HIT_Y_ACTUAL HIT_Y * HIT_Y_MULT
#define BACKGROUND_REDRAW_HEIGHT 20

void drawFallingCircles(int noteIndex, int notesY);
void drawTapIndicator(int noteIndex, int y);


// This function will be used to draw things that might have moved in a frame.
// For example, in a Snake game, draw the snake, the food, the score.
void drawAppState(AppState *state);


// If you have anything else you need accessible from outside the graphics.c
// file, you can add them here. You likely won't.

#endif
