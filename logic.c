#include "logic.h"
#include <stdio.h> 
#include <stdlib.h>

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
    appState->score = score;
    appState->beatsAttempted = 0;
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
        ;
        Song currentSong = songs[appState->currentSongIndex];

        //Keep track of how far in the song we are
        int frameProgress = (vBlankCounter - appState->firstFrameOfThisSong);
        int beatProgress = (frameProgress / currentSong.framesPerBeat);


        //Figure out if the player hit the beat! Only check if they're pressing, and we haven't finished the song
        if(GET_KEY(BUTTON_ANY) && (beatProgress < currentSong.beatCount)) {

            int beatsHit = 0;
            //Check each individual key to see if they hit it
            if(GET_KEY(BUTTON_A)) {
                //Check that we're supposed to be pressing A
                if(currentSong.beatmap[beatProgress + 1] && 0x0800) {
                    beatsHit++;
                } else {
                    //You get a miss if you press the button and there's no notes!
                    appState->score.misses++;
                }
            }



            //How different are you from the optimal?
            int framesOff = ((beatProgress + 1) * currentSong.framesPerBeat) - frameProgress;

            framesOff = abs(framesOff);

            if(framesOff < PERFECT_FRAMES) {
                appState->score.perfects += beatsHit;
            } else if (framesOff < GREAT_FRAMES) {
                appState->score.greats += beatsHit;
            } else if (framesOff < OK_FRAMES) {
                appState->score.oks += beatsHit;
            } else {
                appState->score.misses++;
            }
        }

        //if we've finsihed the song, advance
        if (frameProgress > (currentSong.framesPerBeat * currentSong.beatCount + 100)) {
            //Calculate total score. Oks give you 0 points.
            Score score = appState->score;
            appState->score.totalScore = 100 * score.perfects + 50 * score.greats + -50 * score.misses;

            appState->nextState = SONG_COMPLETE;
        }
        break;


        case SONG_COMPLETE:
        appState->nextState = SONG_COMPLETE_NODRAW;
        break;

        case SONG_COMPLETE_NODRAW:
        //Do a clean restart of the game loop. This way we don't have to worry abt cleaning up score structs or w/e
        if(GET_KEY(BUTTON_ANY)) {
            appState->nextState = APP_INIT;
        }
        break;

        default:
        break;
    }

}
