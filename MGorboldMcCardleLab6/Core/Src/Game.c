/*
 * Game.c
 *
 *  Created on: Apr 17, 2025
 *      Author: mattg
 */


#include "Game.h"

ConnectFourGame Game;
RNG_HandleTypeDef hrng;
static STMPE811_TouchData TouchScreen;

void Game_Init(void){
	TouchScreen.orientation = STMPE811_Orientation_Portrait_2;
	ConnectFour_ResetGame();
	Game.state = STATE_MENU;
	Button_Interrupts_Init();
}

void ConnectFour_ResetGame(void){
	for(int rows = 0; rows < BOARD_ROWS; rows++){
		for(int cols = 0; cols < BOARD_COLS; cols++){
			Game.board[rows][cols] = EMPTY_CELL;
		}
	}

	Game.currentPlayer = PLAYER_ONE;
	Game.currentCol = 4;
	Game.winner = 0;
	Game.isDraw = 0;
	Game.startTime = 0;
}

void ConnectFour_GameLoop(void){
	screen1();

	while(1){
		if(returnTouchStateAndLocation(&TouchScreen) == STMPE811_State_Pressed){
			ConnectFour_ProcessInput(&TouchScreen);
			printf("\nX: %03d\nY: %03d\n", TouchScreen.x, TouchScreen.y);
		}
		HAL_Delay(100);
	}
}

void ConnectFour_ProcessInput(STMPE811_TouchData *touchData){
	uint16_t x = TouchScreen.x;
	uint16_t y = TouchScreen.y;

	switch(Game.state){
	case STATE_MENU:
		// check if 1P or 2P button is pressed on screen1
		if(x >= 45 && x <= 195 && y >= 140 && y <= 175){
			Game.gameMode = GAMEMODE_ONE_PLAYER;
			Game.state = STATE_GAME;
			Game.startTime = 0;
			screen2();
			ConnectFour_DrawPiece();
		}
		else if(x >= 45 && x <= 195 && y >= 70 && y <= 105){
			Game.gameMode = GAMEMODE_TWO_PLAYER;
			Game.state = STATE_GAME;
			Game.startTime = 0;
			screen2();
			ConnectFour_DrawPiece();
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
		ConnectFour_DrawBoard();
		break;

	case STATE_GAME_OVER:
		ConnectFour_GameOver();
		if(x >= 45 && x <= 195 && y >= 210 && y <= 245){
			ConnectFour_ResetGame();
			screen1();
			Game.state = STATE_MENU;
		}
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

void ConnectFour_DrawBoard(void){
	for (int cols = 0; cols < BOARD_COLS; cols++){
		for(int rows = 0; rows < BOARD_ROWS; rows++){
			int x = cols * CELL_SIZE + CELL_SIZE / 2 + 1;
			int y = rows * CELL_SIZE + CELL_SIZE / 2 + 50;
			if(Game.board[rows][cols] == PLAYER_ONE){
				LCD_Draw_Circle_Fill(x,y, PIECE_RADIUS - 2, LCD_COLOR_YELLOW);
			}
			if(Game.board[rows][cols] == PLAYER_TWO){
				LCD_Draw_Circle_Fill(x,y, PIECE_RADIUS - 2, LCD_COLOR_RED);
			}
		}
	}
}

void ConnectFour_GameOver(void){
	LCD_Clear(0,LCD_COLOR_BLACK);
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_SetFont(&Font16x24);
	uint8_t Yellow = Game.YellowWins;
	uint8_t Red = Game.RedWins;
	uint8_t Time = ConnectFour_GetGameTime();
	char one = '0' + (Time / 100);
	char two = '0' + ((Time / 10) % 10);
	char three = '0' + (Time % 10);

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
		LCD_DisplayChar(110, 20, 'I');
		LCD_DisplayChar(130, 20,'N');
		LCD_DisplayChar(150, 20,'S');
		LCD_DisplayChar(170, 20,'!');
	} else{
		LCD_DisplayChar(30, 20, 'P');
		LCD_DisplayChar(50, 20, '2');
		LCD_DisplayChar(90, 20, 'W');
		LCD_DisplayChar(110, 20, 'I');
		LCD_DisplayChar(130, 20,'N');
		LCD_DisplayChar(150, 20,'S');
		LCD_DisplayChar(170, 20,'!');
	}
	LCD_Draw_Vertical_Line(45,75,35,LCD_COLOR_WHITE);
	LCD_Draw_Vertical_Line(195,75,35,LCD_COLOR_WHITE);
	LCD_Draw_Horizontal_Line(45,75,150,LCD_COLOR_WHITE);
	LCD_Draw_Horizontal_Line(45,110,150,LCD_COLOR_WHITE);
	LCD_DisplayChar(50,80,'R');
	LCD_DisplayChar(70,80,'E');
	LCD_DisplayChar(90,80,'S');
	LCD_DisplayChar(110,80,'T');
	LCD_DisplayChar(130,80,'A');
	LCD_DisplayChar(150,80,'R');
	LCD_DisplayChar(170,80,'T');

	LCD_DisplayChar(40,250,'T');
	LCD_DisplayChar(60,250,'I');
	LCD_DisplayChar(80,250,'M');
	LCD_DisplayChar(110,250,'E');
	LCD_DisplayChar(150,250,one); // need to figure out how to make timer
	LCD_DisplayChar(170,250,two);
	LCD_DisplayChar(190,250,three);
	LCD_DisplayChar(210,250,'S');

	LCD_SetTextColor(LCD_COLOR_YELLOW);
	LCD_DisplayChar(40,150,'Y');
	LCD_DisplayChar(60,150,'E');
	LCD_DisplayChar(80,150,'L');
	LCD_DisplayChar(110,150,'W');
	LCD_DisplayChar(130,150,'I');
	LCD_DisplayChar(150,150,'N');
	LCD_DisplayChar(170,150,'S');
	LCD_DisplayChar(200,150,(char)(Yellow + '0'));

	LCD_SetTextColor(LCD_COLOR_RED);
	LCD_DisplayChar(40,200,'R');
	LCD_DisplayChar(60,200,'E');
	LCD_DisplayChar(80,200,'D');
	LCD_DisplayChar(110,200,'W');
	LCD_DisplayChar(130,200,'I');
	LCD_DisplayChar(150,200,'N');
	LCD_DisplayChar(170,200,'S');
	LCD_DisplayChar(200,200,(char)(Red + '0'));
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

static uint8_t CheckPotentialWin(uint8_t col, uint8_t player) {
	// Find the first empty row in the column
	int row = -1;
	for (int r = BOARD_ROWS - 1; r >= 0; r--) {
		if (Game.board[r][col] == EMPTY_CELL) {
			row = r;
			break;
		}
	}
	if (row == -1) return 0; // Column is full
	Game.board[row][col] = player;

	// Check all directions for a win
	uint8_t isWin = 0;

	// Check horizontal
	for (int c = 0; c <= BOARD_COLS - 4; c++) {
		if (Game.board[row][c] == player &&
			Game.board[row][c+1] == player &&
			Game.board[row][c+2] == player &&
			Game.board[row][c+3] == player) {
			isWin = 1;
			break;
		}
	}

	// Check vertical
	if (row <= BOARD_ROWS - 4) {
		if (Game.board[row][col] == player &&
			Game.board[row+1][col] == player &&
			Game.board[row+2][col] == player &&
			Game.board[row+3][col] == player) {
			isWin = 1;
		}
	}

	// Check diagonal (up-right)
	for (int r = 0; r <= BOARD_ROWS - 4; r++) {
		for (int c = 0; c <= BOARD_COLS - 4; c++) {
			if (Game.board[r][c] == player &&
				Game.board[r+1][c+1] == player &&
				Game.board[r+2][c+2] == player &&
				Game.board[r+3][c+3] == player) {
				isWin = 1;
				break;
			}
		}
	}

	// Check diagonal (up-left)
	for (int r = 0; r <= BOARD_ROWS - 4; r++) {
		for (int c = 3; c < BOARD_COLS; c++) {
			if (Game.board[r][c] == player &&
				Game.board[r+1][c-1] == player &&
				Game.board[r+2][c-2] == player &&
				Game.board[r+3][c-3] == player) {
				isWin = 1;
				break;
			}
		}
	}

	// Remove the temporary piece
	Game.board[row][col] = EMPTY_CELL;
	return isWin;
}

static uint8_t CountPiecesInRow(uint8_t col, uint8_t player) {
	int row = -1;
	for (int r = BOARD_ROWS - 1; r >= 0; r--) {
		if (Game.board[r][col] == EMPTY_CELL) {
			row = r;
			break;
		}
	}
	if (row == -1) return 0;

	uint8_t count = 0;
	Game.board[row][col] = player;

	// Check horizontal
	for (int c = 0; c <= BOARD_COLS - 3; c++) {
		uint8_t tempCount = 0;
		for (int i = 0; i < 3; i++) {
			if (Game.board[row][c+i] == player) tempCount++;
		}
		if (tempCount > count) count = tempCount;
	}

	// Check vertical
	if (row <= BOARD_ROWS - 3) {
		uint8_t tempCount = 0;
		for (int i = 0; i < 3; i++) {
			if (Game.board[row+i][col] == player) tempCount++;
		}
		if (tempCount > count) count = tempCount;
	}

	// Check diagonals
	for (int r = 0; r <= BOARD_ROWS - 3; r++) {
		for (int c = 0; c <= BOARD_COLS - 3; c++) {
			uint8_t tempCount = 0;
			for (int i = 0; i < 3; i++) {
				if (Game.board[r+i][c+i] == player) tempCount++;
			}
			if (tempCount > count) count = tempCount;
		}
	}

	for (int r = 0; r <= BOARD_ROWS - 3; r++) {
		for (int c = 2; c < BOARD_COLS; c++) {
			uint8_t tempCount = 0;
			for (int i = 0; i < 3; i++) {
				if (Game.board[r+i][c-i] == player) tempCount++;
			}
			if (tempCount > count) count = tempCount;
		}
	}

	Game.board[row][col] = EMPTY_CELL;
	return count;
}

void ConnectFour_ComputerMove(void) {
	uint8_t bestMove = 0;
	uint8_t bestScore = 0;
	uint8_t validMove = 0;
	uint8_t columns[BOARD_COLS];
	uint32_t random;

	for (uint8_t cols = 0; cols < BOARD_COLS; cols++) {
		columns[cols] = cols;
	}

	for (int cols = BOARD_COLS - 1; cols > 0; cols--) {
		if (HAL_RNG_GenerateRandomNumber(&hrng, &random) == HAL_OK) {
			uint8_t j = random % (cols + 1);
			// Swap columns[i] and columns[j]
			uint8_t temp = columns[cols];
			columns[cols] = columns[j];
			columns[j] = temp;
		}
	}

	// check for winning moves
	for (uint8_t cols = 0; cols < BOARD_COLS; cols++) {
		uint8_t col = columns[cols];
		if (CheckPotentialWin(col, PLAYER_TWO)) {
			if (ConnectFour_DropPiece(col)) {
				Game.currentCol = col;
				return;
			}
		}
	}

	// block winning moves
	for (uint8_t cols = 0; cols < BOARD_COLS; cols++) {
		uint8_t col = columns[cols];
		if (CheckPotentialWin(col, PLAYER_ONE)) {
			if (ConnectFour_DropPiece(col)) {
				Game.currentCol = col;
				return;
			}
		}
	}

	// make a 3 in row
	for (uint8_t cols = 0; cols < BOARD_COLS; cols++) {
		uint8_t col = columns[cols];
		uint8_t score = CountPiecesInRow(col, PLAYER_TWO);
		if (score >= 2 && ConnectFour_DropPiece(col)) {
			Game.currentCol = col;
			return;
		}
	}

	// make a 2 in row
	for (uint8_t cols = 0; cols < BOARD_COLS; cols++) {
		uint8_t col = columns[cols];
		uint8_t score = CountPiecesInRow(col, PLAYER_TWO);
		if (score >= 1 && ConnectFour_DropPiece(col)) {
			Game.currentCol = col;
			return;
		}
	}

	// If no good moves found, make a random move
	while (!validMove) {
		if (HAL_RNG_GenerateRandomNumber(&hrng, &random) == HAL_OK) {
			uint8_t col = random % BOARD_COLS;
			if (ConnectFour_DropPiece(col)) {
				Game.currentCol = col;
				validMove = 1;
			}
		}
	}
}


uint8_t ConnectFour_CheckWin(void){
	// horizontal
	for(int rows = 0; rows < BOARD_ROWS; rows++){
		for(int cols = 0; cols < BOARD_COLS - 3; cols++){
			if(Game.board[rows][cols] == Game.currentPlayer && Game.board[rows][cols+1] == Game.currentPlayer && Game.board[rows][cols+2] == Game.currentPlayer && Game.board[rows][cols+3] == Game.currentPlayer){
				return 1;
			}
		}
	}
	// vertical
	for(int rows = 0; rows < BOARD_ROWS - 3; rows++){
		for(int cols = 0; cols < BOARD_COLS; cols++){
			if(Game.board[rows][cols] == Game.currentPlayer && Game.board[rows+1][cols] == Game.currentPlayer && Game.board[rows+2][cols] == Game.currentPlayer && Game.board[rows+3][cols] == Game.currentPlayer){
				return 1;
			}
		}
	}
	// diagonal upwards to right
	for(int rows = 0; rows < BOARD_ROWS - 3  ; rows++){
		for(int cols = 0; cols < BOARD_COLS - 3; cols++){
			if(Game.board[rows][cols] == Game.currentPlayer && Game.board[rows+1][cols+1] == Game.currentPlayer && Game.board[rows+2][cols+2] == Game.currentPlayer && Game.board[rows+3][cols+3] == Game.currentPlayer){
				return 1;
			}
		}
	}
	// diagonal downwards to right
	for(int rows = 3; rows < BOARD_ROWS; rows++){
		for(int cols = 0; cols < BOARD_COLS - 3; cols++){
			if(Game.board[rows][cols] == Game.currentPlayer && Game.board[rows-1][cols+1] == Game.currentPlayer && Game.board[rows-2][cols+2] == Game.currentPlayer && Game.board[rows-3][cols+3] == Game.currentPlayer){
				return 1;
			}
		}
	}
	return 0;
}

uint8_t ConnectFour_CheckDraw(void){
	for (int cols = 0; cols < BOARD_COLS; cols++){
		for(int rows = 0; rows < BOARD_ROWS; rows++){
			if(Game.board[rows][cols] == EMPTY_CELL){
				return 0;
			}
		}
	}
	return 1;
}

uint8_t ConnectFour_GetGameTime(void) {
	static uint32_t lastTick = 0;
	uint32_t currentTick = HAL_GetTick();
	uint32_t elapsedTicks;
	if (currentTick >= lastTick) {
		elapsedTicks = currentTick - lastTick;
	} else {
		elapsedTicks = ((0xFFFFFFFF - lastTick) + currentTick);
	}
	lastTick = currentTick;
	return (uint8_t)(elapsedTicks / 1000);
}

