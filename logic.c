#include "logic.h"
#include "images/Song1Big.h"
#include "images/Song1Small.h"
#include "images/Song2Big.h"
#include "images/Song2Small.h"
#include "images/Song3Big.h"
#include "images/Song3Small.h"
#include "images/Song4Big.h"
#include "images/Song4Small.h"
#include "images/Song5Big.h"
#include "images/Song5Small.h"
#include "songs/Bokura.h"
#include "songs/Koi.h"
#include "songs/Natsu.h"
#include "songs/Snow.h"
#include "songs/Step.h"


#include "audio.h"
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

//{Song1Audio, SONG1LEN, SONG1FREQ}
Song songs[] = {
    { MAP1_SIZE, 42, map1, Song1Big, Song1Small, "Snow Halation", {Snow, SNOWLEN, SNOWFREQ}},
    { MAP2_SIZE, 34, map2, Song2Big, Song2Small, "Bokura no Live", {Bokura, BOKURALEN, BOKURAFREQ}},
    { MAP3_SIZE, 42, map3, Song3Big, Song3Small, "Natsuiro Egao", {Natsu, NATSULEN, NATSUFREQ}},
    { MAP4_SIZE, 26, map4, Song4Big, Song4Small, "Koi Aqua", {Koi, KOILEN, KOIFREQ}},
    { MAP5_SIZE, 26, map5, Song5Big, Song5Small, "Step! 0 to 1", {Step, STEPLEN, STEPFREQ}}
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
    // int countdown[NOTES_COUNT] = { 0 };
    // memcpy(appState->tapCountdown, countdown, sizeof(appState->tapCountdown));
    //TODO pressing restart causes a 36 bit memory leak each time.
    appState->tapCountdown = (int *) malloc(NOTES_COUNT * sizeof(int));
    int i = 0;
    for(; i < NOTES_COUNT; i++) {
        (appState->tapCountdown)[i] = 0;
    }

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
        playMusic((songs[appState->currentSongIndex]).track);
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

        //Decrement tap progress
        int i = 0;
        for(; i < NOTES_COUNT; i++) {
            (appState->tapCountdown)[i]--;
        }

        //Figure out if the player hit the beat! Only check if they're pressing, and we haven't finished the song
        if(GET_KEY(BUTTON_ANY) && (beatProgress <= currentSong.beatCount)) {

            playHitSfx();

            int beatsHit = 0;
            //Check each individual key to see if they hit it
            //The foreach preprocessor betrayed me :///
            int i = 0;
            for(; i < NOTES_COUNT; i++) {
                Note note = notes[i];
                if(GET_KEY(note.key)) {
                    (appState->tapCountdown)[(note.index)] = HIT_INDICATOR_FRAMES;
                    if(currentSong.beatmap[beatProgress] & note.mapCode) {
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

        // DEBUG
        // drawRectDMA(0, 0, 140, 20, BLACK);
        // char str[25];
        // sprintf(str, "debug: %d", currentSong.framesPerBeat);
        // drawString(0, 0, str, WHITE);

        //if we've finsihed the song, advance
        if (frameProgress > ((currentSong.framesPerBeat * currentSong.beatCount) + 100)) {
            //Calculate total score. Oks give you 0 points.
            Score score = appState->score;
            appState->score.totalScore = 100 * score.perfects + 50 * score.greats + -50 * score.misses;

            appState->nextState = SONG_COMPLETE;
        }
        break;


        case SONG_COMPLETE:
        stopSound();
        playVictorySfx();
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
