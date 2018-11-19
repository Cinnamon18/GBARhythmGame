#include "logic.h"
#include <stdio.h> 
#include <stdlib.h>

Song songs[] = {
    { MAP1_SIZE, 80, map1, CYAN, "Song A"},
    { MAP1_SIZE, 60, map1, MAGENTA, "Song B"},
    { MAP1_SIZE, 40, map1, YELLOW, "Song C"},
    { MAP1_SIZE, 30, map1, GREEN, "Song D"},
    { MAP1_SIZE, 20, map1, BLUE, "Song E"}
};

Note notes[] = {
 { BUTTON_A, 0x0800, 0 },
 { BUTTON_B, 0x0400, 1 },
 { BUTTON_L, 0x0200, 2 },
 { BUTTON_R, 0x0100, 3 },
 { BUTTON_DOWN, 0x0008, 4 },
 { BUTTON_LEFT, 0x0004, 5 },
 { BUTTON_UP, 0x0002, 6 },
 { BUTTON_RIGHT, 0x0001, 7 },
 { BUTTON_START, 0x0010, 8 }
};

void initializeAppState(AppState* appState) {
    appState->nextState = SONG_SELECT;

    appState->currentSongIndex = 0;
    Score score = { 0, 0, 0, 0, 0 };
    appState->score = score;
    appState->tapsThisFrame = 0x0;
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
            appState->nextState = SONG_PLAY_PREP;
        }
        break;
        

        case SONG_PLAY_PREP:
        appState->firstFrameOfThisSong = vBlankCounter + 1;
        appState->nextState = SONG_PLAY;
        break;


        case SONG_PLAY:
        //Main game loop!
        ;
        Song currentSong = songs[appState->currentSongIndex];

        //Keep track of how far in the song we are
        int frameProgress = (vBlankCounter - appState->firstFrameOfThisSong);
        int beatProgress = (frameProgress / currentSong.framesPerBeat);
        int framesIntoThisBeat = frameProgress % currentSong.framesPerBeat;

        //Clear the attempted taps
        appState->tapsThisFrame = 0x0;

        //Figure out if the player hit the beat! Only check if they're pressing, and we haven't finished the song
        if(GET_KEY(BUTTON_ANY) && (beatProgress <= currentSong.beatCount)) {

            int beatsHit = 0;
            //Check each individual key to see if they hit it
            //Honestly? these preprocessors are impressive.
            foreach(Note *note, notes) {
                if(GET_KEY(note->key)) {
                    //make a note that we pressed it for the graphics file
                    appState->tapsThisFrame = appState->tapsThisFrame | note->mapCode;
                    //count up how many notes we've hit
                    if(currentSong.beatmap[beatProgress] && note->mapCode) {
                        beatsHit++;
                    }
                }
            }

            //How different are you from the optimal?
            int framesOff = (currentSong.framesPerBeat - framesIntoThisBeat);
            //We're gonna do a weird thing here bc if you're too slow to hit the note, we might be on the next note, yeah?
            //so if the player is like really super far off, we'll just assume they were too slow.
            if (framesOff > (currentSong.framesPerBeat / 2)) {
                framesOff = (currentSong.framesPerBeat - framesOff);
            }



            //Score appropriately
            if(framesOff < PERFECT_FRAMES) {
                appState->score.perfects += beatsHit;
            } else if (framesOff < GREAT_FRAMES) {
                appState->score.greats += beatsHit;
            } else if (framesOff < OK_FRAMES) {
                appState->score.oks += beatsHit;
            } else {
                appState->score.misses += beatsHit;
            }
        }

        //if we've finsihed the song, advance
        if (frameProgress > ((currentSong.framesPerBeat * currentSong.beatCount) + 100)) {
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
        if(GET_KEY(BUTTON_START)) {
            appState->nextState = APP_INIT;
        }
        break;

        default:
        break;
    }

}
