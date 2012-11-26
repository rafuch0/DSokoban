#ifndef BOARD_H
#define BOARD_H

#include "gba_nds_fat/gba_nds_fat.h"
#include "nds.h"

#define SCREEN_WIDTH 256
#define COL_DEF 32
#define SCREEN_HEIGHT 192
#define ROW_DEF 24
#define INCREMENT 8

using namespace std;

class board
{
	public:
		board();
		~board();
		void movePlayer(int);
		bool isGameDone();
		void draw();
		void loadLevel();
		void nextLevel();
		void drawTile(int i, int j, char type);
		void restartLevel();
		void lastLevel();
		void clear();
		void drawText();

	private:
		char boardGrid[ROW_DEF][COL_DEF];
		int level;
		int jewelsRemaining;
		int playerLocR;
		int playerLocC;
		int readRows;
		int readCols;
		uint16 playerTile[INCREMENT*INCREMENT];
		uint16 wallTile[INCREMENT*INCREMENT];
		uint16 goalTile[INCREMENT*INCREMENT];
		uint16 blockTile[INCREMENT*INCREMENT];
		uint16 completedTile[INCREMENT*INCREMENT];
		uint16 spaceTile[INCREMENT*INCREMENT];

		FAT_FILE *fileData;
		char fatFileName[256];
};

#endif
