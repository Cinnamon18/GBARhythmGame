#include "logic.h"
#include<stdio.h> 


Song songs[] = {
    { MAP1_SIZE, 30, map1, CYAN, "Song A"},
    { MAP1_SIZE, 20, map1, MAGENTA, "Song B"},
    { MAP1_SIZE, 10, map1, YELLOW, "Song C"},
    { MAP1_SIZE, 5, map1, GREEN, "Song D"},
    { MAP1_SIZE, 2, map1, BLUE, "Song E"}
};

void initializeAppState(AppState* appState) {
    appState->nextState = SONG_SELECT;

    appState->currentSongIndex = 1;
    Score score = { 0, 0, 0, 0, 0 };
    appState->currentScore = score;
}

void processAppState(AppState *appState, u32 previousButtons, u32 currentButtons) {

    switch(appState->nextState) {
        case SONG_SELECT:

        //Song selection carousell logic
        if(GET_KEY(BUTTON_ANY_RIGHT)) {
            appState->currentSongIndex++;
        } else if (GET_KEY(BUTTON_ANY_LEFT)) {
            appState->currentSongIndex--;
        }

        appState->currentSongIndex = (appState->currentSongIndex % NUM_SONGS);
        if(appState->currentSongIndex < 0) {
            appState->currentSongIndex = NUM_SONGS - 1;
        }

        //if they hit the start button, advance to main game loop
        if(GET_KEY(BUTTON_START)) {
            appState->firstFrameOfThisSong = vBlankCounter + 1;
            appState->nextState = SONG_PLAY;
        }
        break;
        

        case SONG_PLAY:
        //Main game loop!

        //Keep track of how far in the song we are


        //TODO calculate total score in here
        appState->nextState = SONG_COMPLETE;
        break;


        case SONG_COMPLETE:
        appState->nextState = SONG_COMPLETE_NODRAW;
        break;

        case SONG_COMPLETE_NODRAW:
        if(GET_KEY(BUTTON_ANY)) {
            appState->nextState = SONG_SELECT;
        }
        break;

        default:
        break;
    }

}
