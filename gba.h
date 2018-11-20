#ifndef GBA_SEEN
#define GBA_SEEN

// ---------------------------------------------------------------------------
//                       USEFUL TYPEDEFS
// ---------------------------------------------------------------------------
/** An unsigned 32-bit (4-byte) type */
typedef unsigned int u32;

/** An unsigned 16-bit (2-byte) type */
typedef unsigned short u16;

/** An unsigned 8-bit (1-byte) type. Note that this type cannot be written onto RAM directly. */
typedef unsigned char u8;

// ---------------------------------------------------------------------------
//                       MODE3 MACROS
// ---------------------------------------------------------------------------
#define OFFSET(r, c, rowlen) ((c)+(rowlen)*(r))

#define REG_DISPCNT  *(volatile unsigned short *) 0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)

#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)
#define WHITE COLOR(31,31,31)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define MAGENTA COLOR(31, 0, 31)
#define CYAN COLOR(0, 31, 31)
#define YELLOW COLOR(31, 31, 0)
#define BLACK 0
#define GREY COLOR(5, 5, 5)

//The size of the GBA Screen
#define WIDTH 240
#define HEIGHT 160
#define TOTAL_SCREEN_PIXELS 38400

// This is initialized in gba.c
extern volatile unsigned short *videoBuffer;

// ---------------------------------------------------------------------------
//                       BUTTON INPUT
// ---------------------------------------------------------------------------
#define BUTTON_A			(1<<0)
#define BUTTON_B			(1<<1)
#define BUTTON_SELECT		(1<<2)
#define BUTTON_START		(1<<3)
#define BUTTON_RIGHT		(1<<4)
#define BUTTON_LEFT			(1<<5)
#define BUTTON_UP			(1<<6)
#define BUTTON_DOWN			(1<<7)
#define BUTTON_R			(1<<8)
#define BUTTON_L			(1<<9)
#define BUTTON_ANY			(~0)
#define BUTTON_ANY_LEFT		(BUTTON_A | BUTTON_B | BUTTON_L | BUTTON_R)
#define BUTTON_ANY_RIGHT	(BUTTON_LEFT | BUTTON_UP | BUTTON_DOWN | BUTTON_RIGHT)

#define BUTTONS *(volatile u32 *)0x4000130
#define KEY_DOWN(key, buttons)  (~(buttons) & (key))

// Remember that a button is recently pressed if it wasn't pressed in the last
// input (oldButtons) but is pressed in the current input. Use the KEY_DOWN
// macro to check if the button was pressed in the inputs.
#define KEY_JUST_PRESSED(key, buttons, oldbuttons) (KEY_DOWN(key, buttons) & ~KEY_DOWN(key, oldbuttons))

//My heart tells me this is bad practice, but it sure is convenient. Ah, like a butterfly, fragile and beautiful.
#define GET_KEY(key) KEY_JUST_PRESSED((key), previousButtons, currentButtons)

// ---------------------------------------------------------------------------
//                       DMA
// ---------------------------------------------------------------------------
typedef struct
{
	const volatile void *src;
	const volatile void *dst;
	u32                  cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

// ---------------------------------------------------------------------------
//                       VBLANK
// ---------------------------------------------------------------------------
#define SCANLINECOUNTER (volatile unsigned short *)0x4000006

// Use this variable to count vBlanks. Initialized in gba.c and to be
// manipulated by waitForVBlank()
extern u32 vBlankCounter;

/**
 * Runs a blocking loop until the start of next VBlank.
 */
void waitForVBlank(void);

// ---------------------------------------------------------------------------
//                       MISC
// ---------------------------------------------------------------------------
#define UNUSED(param) ((void)((param)))
void delay(int n);

/**
 * Generates a pseudo-random number between min and max, inclusive.
 *
 * @param  min bottom end of range.
 * @param  max top end of range.
 * @return     random number in the given range.
 */
int randint(int min, int max);

// ---------------------------------------------------------------------------
//                       DRAWING
// ---------------------------------------------------------------------------
void setPixel(int x, int y, u16 color);
void drawRectDMA(int x, int y, int width, int height, volatile u16 color);
void drawFullScreenImageDMA(const u16 *image);
void drawImageDMA(int x, int y, int width, int height, const u16 *image);
void fillScreenDMA(volatile u16 color);
void drawChar(int col, int row, char ch, u16 color);
void drawString(int col, int row, char *str, u16 color);
void drawCenteredString(int col, int row, int width, int height, char *str, u16 color);

/** Contains the pixels of each character from a 6x8 font */
// This is in the font.c file. You can replace the font if you want.
extern const unsigned char fontdata_6x8[12288];







//From 2700 lib:



// Timers
// CONTROLLERS
#define REG_TM0CNT *(volatile unsigned short*)0x4000102
#define REG_TM1CNT *(volatile unsigned short*)0x4000106
#define REG_TM2CNT *(volatile unsigned short*)0x400010A
#define REG_TM3CNT *(volatile unsigned short*)0x400010E

// TIMER VALUES
#define REG_TM0D       *(volatile unsigned short*)0x4000100
#define REG_TM1D       *(volatile unsigned short*)0x4000104
#define REG_TM2D       *(volatile unsigned short*)0x4000108
#define REG_TM3D       *(volatile unsigned short*)0x400010C

// Timer flags
#define TIMER_ON      (1<<7)  
#define TM_IRQ        (1<<6)
#define TM_CASCADE    (1<<2)
#define TM_FREQ_1     0
#define TM_FREQ_64    1
#define TM_FREQ_256   2
#define TM_FREQ_1024  3

// Time factors to multiply clock ticks to convert to microsec (usec)
// The next line (uncommented) should be in myLib.c
// double timefactors[] = {0.059604, 3.811, 15.259, 59.382};
// extern double timefactors[];


// *** Interrupts ====================================================

// CONTROLLER
#define REG_IME *(unsigned short*)0x4000208 
// ENABLER
#define REG_IE *(unsigned short*)0x4000200  
// FLAG
#define REG_IF *(volatile unsigned short*)0x4000202

// 
#define REG_INTERRUPT *(unsigned int*)0x3007FFC
// DISPLAY STATUS
#define REG_DISPSTAT *(unsigned short*)0x4000004

//interrupt constants for turning them on
#define INT_VBLANK_ENABLE 1 << 3

//interrupt constants for checking which type of interrupt happened
#define INT_VBLANK 1 << 0
#define INT_TM1 1<<4
#define INT_BUTTON 1 << 12


// *** Sound =========================================================

#define REG_SOUNDCNT_X *(volatile u16 *)0x04000084

#define PROCESSOR_CYCLES_PER_SECOND (16777216)
#define VBLANK_FREQ (59.727)

// register definitions
#define REG_SOUNDCNT_L        *(u16*)0x04000080
#define REG_SOUNDCNT_H        *(volatile u16*)0x04000082

// flags
#define SND_ENABLED           (1<<7)
#define SND_OUTPUT_RATIO_25   0
#define SND_OUTPUT_RATIO_50   (1<<0)
#define SND_OUTPUT_RATIO_100  (1<<1)
#define DSA_OUTPUT_RATIO_50   (0<<2)
#define DSA_OUTPUT_RATIO_100  (1<<2)
#define DSA_OUTPUT_TO_RIGHT   (1<<8)
#define DSA_OUTPUT_TO_LEFT    (1<<9)
#define DSA_OUTPUT_TO_BOTH    (3<<8)
#define DSA_TIMER0            (0<<10)
#define DSA_TIMER1            (1<<10)
#define DSA_FIFO_RESET        (1<<11)
#define DSB_OUTPUT_RATIO_50   (0<<3)
#define DSB_OUTPUT_RATIO_100  (1<<3)
#define DSB_OUTPUT_TO_RIGHT   (1<<12)
#define DSB_OUTPUT_TO_LEFT    (1<<13)
#define DSB_OUTPUT_TO_BOTH    (3<<12)
#define DSB_TIMER0            (0<<14)
#define DSB_TIMER1            (1<<14)
#define DSB_FIFO_RESET        (1<<15)

// FIFO address defines
#define REG_FIFO_A          (u16*)0x040000A0
#define REG_FIFO_B          (u16*)0x040000A4


typedef volatile struct
{
        volatile const void *src;
        volatile const void *dst;
        volatile unsigned int cnt;
} DMASarah;

extern DMASarah *dma;

void DMANow(int channel, volatile const void* source, volatile const void* destination, unsigned int control);


#endif
