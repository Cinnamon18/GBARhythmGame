#ifndef LOGIC_SEEN
#define LOGIC_SEEN

#include "gba.h"


typedef struct {
    // holds the next state, which main.c reads.
    GBAState nextState;

    Song currentSong;
    Score currentScore;

    int firstFrameOfThisSong;

} AppState;

/*
* TA-TODO: Add any additional structs that you need for your app.
*
* For example, for a Snake game, one could be:
*
* typedef struct {
*   int heading;
*   int length;
*   int x;
*   int y;
* } Snake;
*
*/

typedef struct {
    const unsigned short beatmap[];
    const int beatCount;
    const int framesPerBeat;
} Song;

typedef struct {
    int perfects;
    int greats;
    int oks;
    int misses;
} Score;


// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);

// This function will be used to process app frames.
AppState processAppState(AppState *currentAppState, u32 previousButtons, u32 currentButtons);

// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.

#endif
