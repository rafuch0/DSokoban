#ifndef BOARD_CPP
#define BOARD_CPP

#include "gba_nds_fat/gba_nds_fat.h"
#include <iostream>
#include <stdio.h>
#include <nds/arm9/console.h> //basic print funcionality
#include "board.H"

using namespace std;

#define SCREEN_WIDTH 256
#define COL_DEF 32
#define SCREEN_HEIGHT 192
#define ROW_DEF 24
#define INCREMENT 8

board::board()
{
	level = 1;
	char charBuffer[INCREMENT*INCREMENT*3];
	int readCount;

        FAT_chdir("dsokoban");
	FAT_chdir("tiles");

	fileData = FAT_fopen("playertile", "r");
	readCount = FAT_fread(charBuffer,INCREMENT*INCREMENT*3,1,fileData);
	FAT_fclose(fileData);
	for(int i=0;i<INCREMENT;i++)	{
		for(int j=0;j<INCREMENT;j++)		{
			playerTile[(i*INCREMENT+j)] = RGB15(charBuffer[i*INCREMENT*3+j*3],charBuffer[i*INCREMENT*3+j*3+1],charBuffer[i*INCREMENT*3+j*3+2]);

		}	}

        fileData = FAT_fopen("walltile", "r");
        readCount = FAT_fread(charBuffer,INCREMENT*INCREMENT*3,1,fileData);
        FAT_fclose(fileData);
        for(int i=0;i<INCREMENT;i++)    {
                for(int j=0;j<INCREMENT;j++)            {
                        wallTile[i*INCREMENT+j] = RGB15(charBuffer[i*INCREMENT*3+j*3],charBuffer[i*INCREMENT*3+j*3+1],charBuffer[i*INCREMENT*3+j*3+2]);

                }       }


	fileData = FAT_fopen("goaltile","r");
        readCount = FAT_fread(charBuffer,INCREMENT*INCREMENT*3,1,fileData);
        FAT_fclose(fileData);
        for(int i=0;i<INCREMENT;i++)    {
                for(int j=0;j<INCREMENT;j++)            {
                        goalTile[i*INCREMENT+j] = RGB15(charBuffer[i*INCREMENT*3+j*3],charBuffer[i*INCREMENT*3+j*3+1],charBuffer[i*INCREMENT*3+j*3+2]);

                }       }


        fileData = FAT_fopen("blocktile", "r");
        readCount = FAT_fread(charBuffer,INCREMENT*INCREMENT*3,1,fileData);
        FAT_fclose(fileData);
        for(int i=0;i<INCREMENT;i++)    {
                for(int j=0;j<INCREMENT;j++)            {
                        blockTile[i*INCREMENT+j] = RGB15(charBuffer[i*INCREMENT*3+j*3],charBuffer[i*INCREMENT*3+j*3+1],charBuffer[i*INCREMENT*3+j*3+2]);

                }       }

        fileData = FAT_fopen("completedtile", "r");
        readCount = FAT_fread(charBuffer,INCREMENT*INCREMENT*3,1,fileData);
        FAT_fclose(fileData);
        for(int i=0;i<INCREMENT;i++)    {
                for(int j=0;j<INCREMENT;j++)            {
                        completedTile[i*INCREMENT+j] = RGB15(charBuffer[i*INCREMENT*3+j*3],charBuffer[i*INCREMENT*3+j*3+1],charBuffer[i*INCREMENT*3+j*3+2]);

                }       }

        fileData = FAT_fopen("spacetile", "r");
        readCount = FAT_fread(charBuffer,INCREMENT*INCREMENT*3,1,fileData);
        FAT_fclose(fileData);

        for(int i=0;i<INCREMENT;i++)    {
                for(int j=0;j<INCREMENT;j++)            {
                        spaceTile[i*INCREMENT+j] = RGB15(charBuffer[i*INCREMENT*3+j*3],charBuffer[i*INCREMENT*3+j*3+1],charBuffer[i*INCREMENT*3+j*3+2]);

                }       }

	FAT_chdir("..");
        loadLevel();
}

void board::nextLevel()
{
	level++;
	loadLevel();
}

void board::lastLevel()
{
	level--;
	loadLevel();
}

void board::restartLevel()
{
	loadLevel();
}

void board::loadLevel()
{
	clear();

	char charBuffer[COL_DEF+1];
	int readCount;

	readRows = 0;
	readCols = 0;

	jewelsRemaining = 0;

	fileData = FAT_fopen("levels", "r");

	for(int i=0;i<level;i++)
	{
		do
		{
			readCount = FAT_fread(charBuffer,1,1,fileData);
		}
		while((!(charBuffer[0] == 'L')) && (!(FAT_feof(fileData))));
	}

	do
	{
		readCount = FAT_fread(charBuffer,1,1,fileData);

	}
	while((!(charBuffer[0] == 10)) && (!(FAT_feof(fileData))));

	int cols = 0;
	int rows = 0;
	do
	{
		cols = 0;
		do
		{
			readCount = FAT_fread(charBuffer,1,1,fileData);
			if((charBuffer[0] == 'L') || (FAT_feof(fileData))) break;
			if(charBuffer[0] != 10)	boardGrid[rows][cols] = charBuffer[0];
			cols++;
		}
		while(!(charBuffer[0] == 10));

		if(cols > readCols) readCols = cols;

		rows++;
		if(rows > readRows) readRows = rows;
	}
	while((!(charBuffer[0] == 'L')) && (!(FAT_feof(fileData))));

	readCols--;
	readRows-=2;

	if(FAT_feof(fileData))
	{
		level=1;
		printf("\x1b[23;0HThanks for Playing!"); FAT_FreeFiles(); return;
	}

	FAT_fclose(fileData);

	for(int i=0;i<readRows;i++)
	{
		for(int j=0;j<readCols;j++)
		{
			if(boardGrid[i][j] == '@')
			{
				boardGrid[i][j] = ' ';
				playerLocR = i;
				playerLocC = j;
			}

			if(boardGrid[i][j] == '+')
			{
				boardGrid[i][j] = '.';
				playerLocR = i;
				playerLocC = j;
				jewelsRemaining++;
			}

			if(boardGrid[i][j] == '.')
			{
				jewelsRemaining++;
			}
		}
	}

	draw();
}

board::~board(){}

void board::clear()
{

	for(int i=0;i<ROW_DEF;i++)
	{
		for(int j=0;j<COL_DEF;j++)
		{
			drawTile(i,j, ' ');
			boardGrid[i][j]=' ';
			printf("\x1b[%d;%dH ",i,j);
		}
	}

	playerLocR = 0;
	playerLocC = 0;
	draw();
}




void board::drawTile(int i, int j, char type)
{
	i*=INCREMENT;
	j*=INCREMENT;

	for(int y=0;y<INCREMENT;y++)
        {
        	for(int x=0;x<INCREMENT;x++)
                {
			switch(type)
			{
				case '#':
					VRAM_A[(i+y)*SCREEN_WIDTH+j+x] = wallTile[y*INCREMENT+x]|0x8000;
					break;

				case '*':
	                                VRAM_A[(i+y)*SCREEN_WIDTH+j+x] = completedTile[y*INCREMENT+x]|0x8000;
	                                break;

				case '$':
	                                VRAM_A[(i+y)*SCREEN_WIDTH+j+x] = blockTile[y*INCREMENT+x]|0x8000;
	                                break;

				case '.':
	                                VRAM_A[(i+y)*SCREEN_WIDTH+j+x] = goalTile[y*INCREMENT+x]|0x8000;
	                                break;

				case ' ':
	                                VRAM_A[(i+y)*SCREEN_WIDTH+j+x] = spaceTile[y*INCREMENT+x]|0x8000;
	                                break;

				case 'P':
	                                VRAM_A[(i+y)*SCREEN_WIDTH+j+x] = playerTile[y*INCREMENT+x]|0x8000;
	                                break;

				default:
					break;
			}
		}
	}
}

void board::drawText()
{
        for(int i=0;i<readRows;i++)
        {
                for(int j=0;j<readCols;j++)
                {
                        printf("\x1b[%d;%dH%c",i,j,boardGrid[i][j]);
                        if(((i)==playerLocR) && ((j)==playerLocC)) printf("\x1b[%d;%dH%c",i,j,'O');
		}
	}
}


void board::draw()
{
	drawText();
        for(int i=0;i<readRows;i++)
        {
                for(int j=0;j<readCols;j++)
                {
			switch (boardGrid[i][j])
			{
				case '#':
					drawTile(i,j, '#');
					break;
				case '*':
					drawTile(i,j, '*');
					break;
				case '$':
					drawTile(i,j, '$');
					break;
				case '.':
					drawTile(i,j, '.');
					break;
				case ' ':
					drawTile(i,j, ' ');
					break;
				default:
					break;
			}

			if( ((i)==playerLocR) && ((j) == playerLocC))
			{
				drawTile(i,j,'P');
			}
		}
	}
return;
}

void board::movePlayer(int direction)
{
	int pushR = playerLocR;
	int pushC = playerLocC;
	int moveR = playerLocR;
	int moveC = playerLocC;

	switch (direction)
	{
		case 0:
			pushR = playerLocR - 2;
			moveR = playerLocR - 1;
			break;

		case 1:
			pushR = playerLocR + 2;
			moveR = playerLocR + 1;
			break;
		case 2:
			pushC = playerLocC - 2;
			moveC = playerLocC - 1;
			break;
		case 3:
			pushC = playerLocC + 2;
			moveC = playerLocC + 1;
			break;

		default:
			return;
	}


        if((moveR < 0) || (moveR > ROW_DEF - 1) || (moveC < 0) || (moveC > COL_DEF - 1)) return;
        if(boardGrid[moveR][moveC] == '#') return;

        if((boardGrid[moveR][moveC] == ' ') || (boardGrid[moveR][moveC] == '.'))
        {
		drawTile(playerLocR,playerLocC, boardGrid[playerLocR][playerLocC]);
                playerLocR = moveR;
                playerLocC = moveC;
		drawTile(playerLocR,playerLocC,'P');
		drawText();
                return;
        }

	if((pushR < 0) || (pushR > ROW_DEF - 1) || (pushC < 0) || (pushC > COL_DEF - 1)) return;
	if(!((boardGrid[moveR][moveC] == '$') || (boardGrid[moveR][moveC] == '*'))) return;
	if(!((boardGrid[pushR][pushC] == ' ') || (boardGrid[pushR][pushC] == '.'))) return;

	if(boardGrid[moveR][moveC] == '*')
	{
		if(boardGrid[pushR][pushC] == ' ')
		{
			boardGrid[pushR][pushC] = '$';
			boardGrid[moveR][moveC] = '.';
			jewelsRemaining++;
		}

		if(boardGrid[pushR][pushC] == '.')
		{
			boardGrid[pushR][pushC] = '*';
			boardGrid[moveR][moveC] = '.';
		}
	}

	if(boardGrid[moveR][moveC] == '$')
	{
		if(boardGrid[pushR][pushC] == ' ')
		{
			boardGrid[pushR][pushC] = '$';
			boardGrid[moveR][moveC] = ' ';
		}
		if(boardGrid[pushR][pushC] == '.')
		{
			boardGrid[pushR][pushC] = '*';
			boardGrid[moveR][moveC] = ' ';
			jewelsRemaining--;
		}
	}

	drawTile(playerLocR,playerLocC,boardGrid[playerLocR][playerLocC]);
	drawTile(pushR,pushC,boardGrid[pushR][pushC]);

        playerLocR = moveR;
        playerLocC = moveC;

	drawTile(playerLocR,playerLocC,'P');
	drawText();
}


bool board::isGameDone()
{
	return (jewelsRemaining == 0);
}

#endif
