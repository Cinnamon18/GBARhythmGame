#ifndef LOGIC_SEEN
#define LOGIC_SEEN

#include "gba.h"
#include "beatmaps.h"

// AppState enum definition
typedef enum {
    // TA-TODO: Add any additional states you need for your app.
    START,
    START_NODRAW,
    APP_INIT,
    SONG_SELECT,
    SONG_PLAY,
    SONG_COMPLETE
} GBAState;

typedef struct {
    const int beatCount;
    const int framesPerBeat;
    const unsigned short * beatmap;
} Song;

typedef struct {
    int perfects;
    int greats;
    int oks;
    int misses;
} Score;

typedef struct {
    // holds the next state, which main.c reads.
    GBAState nextState;

    Song currentSong;
    Score currentScore;

    int firstFrameOfThisSong;

} AppState;


// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);

// This function will be used to process app frames.
void processAppState(AppState *currentAppState, u32 previousButtons, u32 currentButtons);

// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.

#endif
