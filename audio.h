#ifndef AUDIO_SEEN
#define AUDIO_SEEN

#include "gba.h"
#include "logic.h"

typedef struct{
	const unsigned char* data;
	int length;
	int frequency;
	int isPlaying;
	int loops;
	int duration;
	int priority;
	int vBlankCount;
} SOUND;

void setupAudio(void);
void playMusic(Track track);
void playHitSfx(void);
void playVictorySfx(void);


// SOUND soundA;
// SOUND soundB;

void setupSounds(void);
void playSoundA( const unsigned char* sound, int length, int frequency, int loops);
void playSoundB( const unsigned char* sound, int length, int frequency, int loops);

void setupInterrupts(void);
void interruptHandler(void);

void pauseSound(void);
void unpauseSound(void);
void stopSound(void);


#endif
