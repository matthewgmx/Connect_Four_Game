/*
 * Game.h
 *
 *  Created on: Apr 17, 2025
 *      Author: mattg
 */

#ifndef INC_GAME_H_
#define INC_GAME_H_

#include "LCD_Driver.h"
#include "Button_Driver.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Game constants
#define BOARD_COLS 			7
#define BOARD_ROWS 			6 // 1,2,3,4,5,6
#define PLAYER_ONE 			1
#define PLAYER_TWO 			2
#define COMPUTER_PLAYER 	2
#define EMPTY_CELL 			0
#define GAMEMODE_ONE_PLAYER 1
#define GAMEMODE_TWO_PLAYER 2

// Screen dimensions
#define SCREEN_WIDTH 		LCD_PIXEL_WIDTH
#define SCREEN_HEIGHT 		LCD_PIXEL_HEIGHT
#define BOARD_MARGIN_Y 		204
#define BOARD_MARGIN_X		240
#define CELL_SIZE 			34
#define PIECE_RADIUS 		(CELL_SIZE / 2) - 2



typedef enum {
    STATE_MENU,
    STATE_GAME,
    STATE_GAME_OVER
} GameState;

typedef struct {
    uint8_t board[BOARD_ROWS][BOARD_COLS];
    uint8_t currentPlayer;
    uint8_t gameMode;
    uint8_t currentCol; // Colums from 1,2,3,4,5,6,7
    GameState state;
    uint8_t winner;
    uint8_t isDraw;
    uint8_t YellowWins;
    uint8_t RedWins;
} ConnectFourGame;

ConnectFourGame Game;
RNG_HandleTypeDef hrng;

void Game_Init(void); //
void ConnectFour_GameLoop(void); //
void ConnectFour_DrawPiece(void); //
void ConnectFour_GameOver(void); //
void ConnectFour_ProcessInput(STMPE811_TouchData *touchData); //
void ConnectFour_ComputerMove(void); //
uint8_t ConnectFour_DropPiece(uint8_t col); //
uint8_t ConnectFour_CheckWin(void);
uint8_t ConnectFour_CheckDraw(void);
void ConnectFour_ResetGame(void); //

#endif /* INC_GAME_H_ */
