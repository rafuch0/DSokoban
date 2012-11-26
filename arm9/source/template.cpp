#include "nds.h"
#include <nds/arm9/console.h> //basic print funcionality
#include <stdio.h>
#include "gba_nds_fat/gba_nds_fat.h"
#include <iostream>
#include "board.H"
#include "screenshot.h"

using namespace std;

int main(void)
{

	irqInit();
	irqSet(IRQ_VBLANK, 0);

	videoSetMode(MODE_FB0);	//not using the main screen
	vramSetBankA(VRAM_A_LCD);
	vramSetBankB(VRAM_B_LCD);

	videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE);	//sub bg 0 will be used to print text
	vramSetBankC(VRAM_C_SUB_BG);

	SUB_BG0_CR = BG_MAP_BASE(31);

	BG_PALETTE_SUB[255] = RGB15(31,31,31);	//by default font will be rendered with color 255

	consoleInitDefault((u16*)SCREEN_BASE_BLOCK_SUB(31), (u16*)CHAR_BASE_BLOCK_SUB(0), 16);

	FAT_InitFiles();
	for(int i=0;i<10;i++) swiWaitForVBlank();
	board theBoard;
	uint16 keysPressed;

	theBoard.draw();
	while(1)
	{

		keysPressed = ~(REG_KEYINPUT);

		if(keysPressed & KEY_UP) {theBoard.movePlayer(0); for(int i=0;i<7;i++)swiWaitForVBlank();}
		if(keysPressed & KEY_DOWN) {theBoard.movePlayer(1); for(int i=0;i<7;i++)swiWaitForVBlank();}
		if(keysPressed & KEY_LEFT) {theBoard.movePlayer(2); for(int i=0;i<7;i++)swiWaitForVBlank();}
		if(keysPressed & KEY_RIGHT) {theBoard.movePlayer(3); for(int i=0;i<7;i++)swiWaitForVBlank();}

		if(keysPressed & KEY_L) theBoard.nextLevel();
		if(keysPressed & KEY_R) theBoard.lastLevel();
		if(keysPressed & KEY_START) theBoard.restartLevel();
		if(keysPressed & KEY_SELECT) break;
		if(keysPressed & KEY_A) screenshotbmp("screenshot.bmp");

		if(theBoard.isGameDone()) theBoard.nextLevel();
	}

	printf("\x1b[23;0HThanks For Playing!");
	FAT_FreeFiles();

	return 0;
}
