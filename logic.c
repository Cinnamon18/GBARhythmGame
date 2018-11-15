#include "logic.h"
#include<stdio.h> 


Song songs[] = {
    { MAP1_SIZE, 30, map1, CYAN},
    { MAP1_SIZE, 20, map1, MAGENTA},
    { MAP1_SIZE, 10, map1, YELLOW},
    { MAP1_SIZE, 5, map1, GREEN},
    { MAP1_SIZE, 2, map1, BLUE}
};

void initializeAppState(AppState* appState) {
    appState->nextState = SONG_SELECT;

    appState->currentSongIndex = 1;
}

// TA-TODO: Add any process functions for sub-elements of your app here.
// For example, for a snake game, you could have a processSnake function
// or a createRandomFood function or a processFoods function.
//
// e.g.:
// static Snake processSnake(Snake* currentSnake);
// static void generateRandomFoods(AppState* currentAppState, AppState* nextAppState);

// This function processes your current app state and returns the new (i.e. next)
// state of your application.
void processAppState(AppState *currentAppState, u32 previousButtons, u32 currentButtons) {

    AppState appState = *currentAppState;

    switch(appState.nextState) {
        case SONG_SELECT:

        if(GET_KEY(BUTTON_ANY_RIGHT)) {
            appState.currentSongIndex++;
        } else if (GET_KEY(BUTTON_ANY_LEFT)) {
            appState.currentSongIndex--;
        }

        appState.currentSongIndex = (appState.currentSongIndex % NUM_SONGS);

        drawRectDMA(0, 0, 100, 20, WHITE);
        char str[20];
        sprintf(str, "song index: %d", appState.currentSongIndex);
        drawString(0, 0, str, BLACK);

        if(GET_KEY(BUTTON_START)) {
            // appState.nextState = SONG_PLAY;
        }
        break;
        

        case SONG_PLAY:

        appState.nextState = SONG_COMPLETE;
        break;


        case SONG_COMPLETE:
        if(GET_KEY(BUTTON_ANY)) {
            appState.nextState = SONG_SELECT;
        }
        break;

        default:
        break;
    }

}
