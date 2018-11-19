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
    SONG_PLAY_PREP,
    SONG_PLAY,
    SONG_COMPLETE,
    SONG_COMPLETE_NODRAW
} GBAState;

typedef struct {
    const int beatCount;
    const int framesPerBeat;
    const unsigned short * beatmap;
    const u16 previewImage;
    char name[15];
} Song;

typedef struct {
    const u16 key;
    const u16 mapCode;
    const int index;
} Note;

typedef struct {
    int perfects;
    int greats;
    int oks;
    int misses;
    int totalScore;
} Score;

extern Song songs[5];
extern Note notes[9];
#define NUM_SONGS 5
#define PERFECT_FRAMES 4
#define GREAT_FRAMES 8
#define OK_FRAMES 16



#define foreach(item, array)                                \
    for(int keep = 1,                                       \
        count = 0,                                          \
        size = sizeof (array) / sizeof *(array);            \
        keep && count != size;                              \
        keep = !keep, count++)                              \
    for(item = (array) + count; keep; keep = !keep)


typedef struct {
    // general
    GBAState nextState;

    //song select
    int currentSongIndex;

    //song play
    int firstFrameOfThisSong;
    Score score;
    u16 tapsThisFrame; //Keeps track of which notes the player has pressed

    //song complete

} AppState;


// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);

// This function will be used to process app frames.
void processAppState(AppState *currentAppState, u32 previousButtons, u32 currentButtons);

// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.

#endif
