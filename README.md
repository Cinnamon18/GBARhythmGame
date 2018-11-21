# Cinnamon's GBA Rhythm Game

A rhythm game I made for the CS 2110 GBA project.

***Crank that volume i worked so hard on audio even though it ended up kinda shitty***

Please note I rebound the default mednafen controls. Here are the relevant keybinds:
```
|-|-|-|-|-|-|-|-|-|
|A|S|D|F| |J|K|L|;|
|-|-|-|-|-|-|-|-|-|
  left      right
```
When playing a song there are 8 possible places a note can fall. Press the keyboard key that corresponds to the note. That is to say, the leftmost note spot corresponds to A, the second leftmost to S, and so on.

On the song select screen, press the left keys to scroll left, and the right keys to scroll right. Pretty simple! Space bar is the "confirm" or "enter" button. Select is bound to its default key, "backspace", because I was afraid of getting marked down on the "select must soft reset the game" requirement if a TA couldn't figure out my more thematically consistent rebind :p

Scoring is tallied as follows:
* Perfect: 100 points
* Great: 50 points
* OK: 0 points
* Miss: -50 points

## Build instructions
Try playing the .gba. If that doesn't work, you should only need a simple `make emu` to run. However, I could be mistaken. If so, check HW10.pdf for instructions.