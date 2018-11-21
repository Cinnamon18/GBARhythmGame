#include "gba.h"
#include "logic.h"
#include "audio.h"
#include "songs/TitleSong.h"
#include "songs/HitNoise.h"
#include "songs/Victory.h"


SOUND soundA;
SOUND soundB;

void setupAudio(void) {
	setupSounds();
	//TODO. So uh. I think i can't stop audio without this line, but it was hanging after the _completion_ of the first main() call. 
	//To be clear. The entire main() call executes (provided you don't put it across multiple frames), and then it hangs.
	// setupInterrupts();
}

void playMusic(Track track) {
	playSoundA(track.data, track.length, track.frequency, 0);
}

//TODO
void playHitSfx(void) {
	//i forgot to mono it oops
	playSoundB(HitNoise, HITNOISELEN, HITNOISEFREQ, 0);
}

//TODO
void playVictorySfx(void) {
	playSoundB(Victory_c1, VICTORYLEN, VICTORYFREQ, 0);
}





//Boilerplate code love you sarah.

void setupSounds(void)
{
	REG_SOUNDCNT_X = SND_ENABLED;

	REG_SOUNDCNT_H = SND_OUTPUT_RATIO_100 | 
	DSA_OUTPUT_RATIO_100 | 
	DSA_OUTPUT_TO_BOTH | 
	DSA_TIMER0 | 
	DSA_FIFO_RESET |
	DSB_OUTPUT_RATIO_100 | 
	DSB_OUTPUT_TO_BOTH | 
	DSB_TIMER1 | 
	DSB_FIFO_RESET;

	REG_SOUNDCNT_L = 0;
}

void playSoundA( const unsigned char* sound, int length, int frequency, int loops) {
	dma[1].cnt = 0;
	
	int ticks = PROCESSOR_CYCLES_PER_SECOND/frequency;
	
	DMANow(1, sound, REG_FIFO_A, DMA_DESTINATION_FIXED | DMA_AT_REFRESH | DMA_REPEAT | DMA_32);
	
	REG_TM0CNT = 0;
	
	REG_TM0D = -ticks;
	REG_TM0CNT = TIMER_ON;

	soundA.data = sound;
	soundA.length = length;
	soundA.frequency = frequency;
	soundA.loops = loops;
	soundA.isPlaying = 1;
	soundA.duration = ((VBLANK_FREQ * soundA.length)/soundA.frequency);
	soundA.vBlankCount = 0;    
}


void playSoundB( const unsigned char* sound, int length, int frequency, int loops) {
	dma[2].cnt = 0;
	
	int ticks = PROCESSOR_CYCLES_PER_SECOND/frequency;

	DMANow(2, sound, REG_FIFO_B, DMA_DESTINATION_FIXED | DMA_AT_REFRESH | DMA_REPEAT | DMA_32);

	REG_TM1CNT = 0;
	
	REG_TM1D = -ticks;
	REG_TM1CNT = TIMER_ON;
	
	soundB.data = sound;
	soundB.length = length;
	soundB.frequency = frequency;
	soundB.loops = loops;
	soundB.isPlaying = 1;
	soundB.duration = ((VBLANK_FREQ * soundB.length)/soundB.frequency);
	soundB.vBlankCount = 0;
}

void setupInterrupts(void)
{
	REG_IME = 0;
	REG_INTERRUPT = (unsigned int) interruptHandler;
	REG_IE |= INT_VBLANK;
	REG_DISPSTAT |= INT_VBLANK_ENABLE;
	REG_IME = 1;
}

void interruptHandler(void) {
	REG_IME = 0;

	if(REG_IF & INT_VBLANK) {
		if (soundA.isPlaying) {
			soundA.vBlankCount++;
			if (soundA.vBlankCount > soundA.duration) {
				if (soundA.loops) {
					playSoundA(soundA.data, soundA.length, soundA.frequency, soundA.loops);
				} else {
					soundA.isPlaying = 0;
					dma[1].cnt = 0;
					REG_TM0CNT = 0;
				}
			}
		}

		if (soundB.isPlaying) {
			soundB.vBlankCount++;
			if (soundB.vBlankCount > soundB.duration) {
				if (soundB.loops) {
					playSoundB(soundB.data, soundB.length, soundB.frequency, soundB.loops);
				} else {
					soundB.isPlaying = 0;
					dma[2].cnt = 0;
					REG_TM1CNT = 0;
				}
			}
		}
		
		REG_IF = INT_VBLANK; 
	}
	REG_IME = 1;
}

void stopSound(void) {
	soundA.isPlaying = 0;
	dma[1].cnt = 0;
	REG_TM0CNT = 0;
	soundB.isPlaying = 0;
	dma[2].cnt = 0;
	REG_TM1CNT = 0;
}