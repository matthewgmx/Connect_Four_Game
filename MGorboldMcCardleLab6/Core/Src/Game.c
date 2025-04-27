/*
 * Game.c
 *
 *  Created on: Apr 17, 2025
 *      Author: mattg
 */


#include "Game.h"

static STMPE811_TouchData TouchScreen;

void Game_Init(void){
	TouchScreen.orientation = STMPE811_Orientation_Portrait_2;
	ConnectFour_ResetGame();
	Game.state = STATE_MENU;
	Button_Interrupts_Init();
}

void ConnectFour_ResetGame(void){
	memset(Game.board, EMPTY_CELL, sizeof(Game.board));
	Game.currentPlayer = PLAYER_ONE;
	Game.currentCol = 4;
	Game.winner = 0;
	Game.isDraw = 0;
}

void ConnectFour_GameLoop(void){
	screen1();

	while(1){
		if(returnTouchStateAndLocation(&TouchScreen) == STMPE811_State_Pressed){
			ConnectFour_ProcessInput(&TouchScreen);
			printf("\nX: %03d\nY: %03d\n", TouchScreen.x, TouchScreen.y);
		}
		HAL_Delay(50);
	}
}

void ConnectFour_ProcessInput(STMPE811_TouchData *touchData){
	uint16_t x = TouchScreen.x;
	uint16_t y = TouchScreen.y;

	switch(Game.state){
	case STATE_MENU:
		// check if 1P or 2P button is pressed on screen1
		if(x >= 45 && x <= 195 && y >= 145 && y <= 180){
			Game.gameMode = GAMEMODE_ONE_PLAYER;
			Game.state = STATE_GAME;
			screen2();
			ConnectFour_DrawPiece();
		}
		else if(x >= 45 && x <= 195 && y >= 215 && y <= 250){
			Game.gameMode = GAMEMODE_TWO_PLAYER;
			Game.state = STATE_GAME;
			screen2();
		}
		break;
	case STATE_GAME:

		if (x < BOARD_MARGIN_X / 2) {
			if (Game.currentCol > 0) {
				Game.currentCol--;
			}
		}
		// Right side of screen - move piece right
		else if (x >= BOARD_MARGIN_X / 2) {
			if (Game.currentCol < BOARD_COLS - 1) {
				Game.currentCol++;
			}
		}
		screen2();
		ConnectFour_DrawPiece();
		break;
	case STATE_GAME_OVER:
		ConnectFour_GameOver();
		break;
	}
}

void ConnectFour_DrawPiece(void) {
	int x = Game.currentCol * CELL_SIZE + CELL_SIZE / 2  + 1;
	int y = 25;
	if (Game.currentPlayer == PLAYER_ONE) {
		LCD_Draw_Circle_Fill(x, y, PIECE_RADIUS, LCD_COLOR_YELLOW);
	} else {
		LCD_Draw_Circle_Fill(x, y, PIECE_RADIUS, LCD_COLOR_RED);
	}
}

void ConnectFour_GameOver(void){
	LCD_Clear(0,LCD_COLOR_BLACK);
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_SetFont(&Font16x24);
	char Yellow = Game.YellowWins;
	char Red = Game.RedWins;

	if(Game.isDraw){
		LCD_DisplayChar(90, 20, 'D');
		LCD_DisplayChar(110, 20, 'R');
		LCD_DisplayChar(130, 20, 'A');
		LCD_DisplayChar(150, 20, 'W');
		LCD_DisplayChar(170,20,'!');

	} else if(Game. winner == PLAYER_ONE){
		LCD_DisplayChar(30, 20, 'P');
		LCD_DisplayChar(50, 20, '1');
		LCD_DisplayChar(90, 20, 'W');
		LCD_DisplayChar(90, 20, 'I');
		LCD_DisplayChar(110, 20,'N');
		LCD_DisplayChar(130, 20,'S');
		LCD_DisplayChar(150, 20,'!');
	} else{
		LCD_DisplayChar(30, 20, 'P');
		LCD_DisplayChar(50, 20, '2');
		LCD_DisplayChar(90, 20, 'W');
		LCD_DisplayChar(90, 20, 'I');
		LCD_DisplayChar(110, 20,'N');
		LCD_DisplayChar(130, 20,'S');
		LCD_DisplayChar(150, 20,'!');
	}
	LCD_Draw_Vertical_Line(45,105,35,LCD_COLOR_WHITE);
	LCD_Draw_Vertical_Line(195,105,35,LCD_COLOR_WHITE);
	LCD_Draw_Horizontal_Line(45,105,150,LCD_COLOR_WHITE);
	LCD_Draw_Horizontal_Line(45,140,150,LCD_COLOR_WHITE);
	LCD_DisplayChar(50,110,'R');
	LCD_DisplayChar(70,110,'E');
	LCD_DisplayChar(90,110,'S');
	LCD_DisplayChar(110,110,'T');
	LCD_DisplayChar(130,110,'A');
	LCD_DisplayChar(150,110,'R');
	LCD_DisplayChar(170,110,'T');

	LCD_DisplayChar(40,250,'T');
	LCD_DisplayChar(60,250,'I');
	LCD_DisplayChar(80,250,'M');
	LCD_DisplayChar(110,250,'E');
	LCD_DisplayChar(170,200,'0'); // need to figure out how to make timer

	LCD_SetTextColor(LCD_COLOR_YELLOW);
	LCD_DisplayChar(40,150,'Y');
	LCD_DisplayChar(60,150,'E');
	LCD_DisplayChar(80,150,'L');
	LCD_DisplayChar(110,150,'W');
	LCD_DisplayChar(130,150,'I');
	LCD_DisplayChar(150,150,'N');
	LCD_DisplayChar(170,150,'S');
	LCD_DisplayChar(170,150,Yellow);

	LCD_SetTextColor(LCD_COLOR_RED);
	LCD_DisplayChar(40,200,'R');
	LCD_DisplayChar(60,200,'E');
	LCD_DisplayChar(80,200,'D');
	LCD_DisplayChar(110,200,'W');
	LCD_DisplayChar(130,200,'I');
	LCD_DisplayChar(150,200,'N');
	LCD_DisplayChar(170,200,'S');
	LCD_DisplayChar(170,200,Red);


}


uint8_t ConnectFour_DropPiece(uint8_t col){
	for (int rows = BOARD_ROWS - 1; rows >= 0; rows--) {
		if (Game.board[rows][col] == EMPTY_CELL) {
			Game.board[rows][col] = Game.currentPlayer;
			return 1;
		}
	}
	return 0;
}

void ConnectFour_ComputerMove(void){
	uint32_t random;
	uint8_t col;
	uint8_t valid;
	if(HAL_RNG_GenerateRandomNumber(&hrng, &random) == HAL_OK){
		col = random % BOARD_COLS;
		if(ConnectFour_DropPiece(col)){
			return;
		}
	}
	while(!valid){			// if column is full
		col = rand() % BOARD_COLS;
		valid = ConnectFour_DropPiece(col);
	}
}

uint8_t ConnectFour_CheckWin(void){
	// horizontal
	for(int rows = 1; rows <= BOARD_ROWS; rows++){
		for(int cols = 1; cols <= BOARD_COLS - 3; cols++){
			if(Game.board[rows][cols] == Game.currentPlayer && Game.board[rows][cols+1] == Game.currentPlayer && Game.board[rows][cols+2] == Game.currentPlayer && Game.board[rows][cols+3] == Game.currentPlayer){
				return 0;
			}
		}
	}
	// vertical
	for(int rows = 1; rows <= BOARD_ROWS-3; rows++){
		for(int cols = 1; cols <= BOARD_COLS; cols++){
			if(Game.board[rows][cols] == Game.currentPlayer && Game.board[rows+1][cols] == Game.currentPlayer && Game.board[rows+2][cols] == Game.currentPlayer && Game.board[rows+3][cols] == Game.currentPlayer){
				return 0;
			}
		}
	}
	// diagonal upwards to right
	for(int rows = 1; rows <= BOARD_ROWS -3  ; rows++){
		for(int cols = 1; cols <= BOARD_COLS - 3; cols++){
			if(Game.board[rows][cols] == Game.currentPlayer && Game.board[rows+1][cols+1] == Game.currentPlayer && Game.board[rows+2][cols+2] == Game.currentPlayer && Game.board[rows+3][cols+3] == Game.currentPlayer){
				return 0;
			}
		}
	}
	// diagonal downwards to right
	for(int rows = 4; rows <= BOARD_ROWS; rows++){
		for(int cols = 1; cols <= BOARD_COLS - 3; cols++){
			if(Game.board[rows][cols] == Game.currentPlayer && Game.board[rows-1][cols+1] == Game.currentPlayer && Game.board[rows-2][cols+2] == Game.currentPlayer && Game.board[rows-3][cols+3] == Game.currentPlayer){
				return 0;
			}
		}
	}
	return 01;
}

uint8_t ConnectFour_CheckDraw(void){
	for (int cols = 1; cols <= BOARD_COLS; cols++){
		for(int rows = 1; rows <= BOARD_ROWS; rows++){
			if(Game.board[rows][cols] == EMPTY_CELL){
				return 0;
			}
		}
	}
	return 1;
}



