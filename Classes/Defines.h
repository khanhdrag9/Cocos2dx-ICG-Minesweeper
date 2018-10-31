#pragma once

//GameStart
#define TITLE "Minesweeper by T.N.Khanh"
#define PLAY_LABEL "Touch on screen to play!"

//Gameplay
#define TEST_GRID 1

#define BACKGROUND "gridsBoard.jpg"
#define SPRITE_PLIST "sprites.plist"
#define GRID_HOVER "grid.png"
#define GRID_SHOWN "gridShown.png"
#define GIRD_MINE "mine.png"
#define GRID_SIZE 64
#define RATIO_MINE_WITH_NUMBER_GRIDS 0.15F

#define FONT_PATH "fonts/arial.ttf"

//Result game
#define WIN_LABEL "WIN :D"
#define LOSE_LABEL "LOSE :("
#define RESET_LABEL "Touch on screen to play again"

typedef enum Levels
{
	EASY = 4,
	NORMAL = 9,
	HARD = 12
}Levels;