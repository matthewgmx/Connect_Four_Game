/*
 * ApplicationCode.c
 *
 *  Created on: Dec 30, 2023 (updated 11/12/2024) Thanks Donavon! 
 *      Author: Xavion
 */

#include "ApplicationCode.h"

/* Static variables */


extern void initialise_monitor_handles(void); 

#if COMPILE_TOUCH_FUNCTIONS == 1
static STMPE811_TouchData StaticTouchData;
#endif // COMPILE_TOUCH_FUNCTIONS

void ApplicationInit(void)
{
	initialise_monitor_handles(); // Allows printf functionality
    LTCD__Init();
    LTCD_Layer_Init(0);
    LCD_Clear(0,LCD_COLOR_WHITE);
    Game_Init();
    #if COMPILE_TOUCH_FUNCTIONS == 1
	InitializeLCDTouch();

	// This is the orientation for the board to be direclty up where the buttons are vertically above the screen
	// Top left would be low x value, high y value. Bottom right would be low x value, low y value.
	StaticTouchData.orientation = STMPE811_Orientation_Portrait_2;

	#endif // COMPILE_TOUCH_FUNCTIONS
}

void LCD_Visual_Demo(void)
{
	visualDemo();
}

#if COMPILE_TOUCH_FUNCTIONS == 1
void LCD_Touch_Polling_Demo(void)
{
	LCD_Clear(0,LCD_COLOR_GREEN);
	while (1) {
		/* If touch pressed */
		if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
			/* Touch valid */
			printf("\nX: %03d\nY: %03d\n", StaticTouchData.x, StaticTouchData.y);
			LCD_Clear(0, LCD_COLOR_RED);
		} else {
			/* Touch not pressed */
			printf("Not Pressed\n\n");
			LCD_Clear(0, LCD_COLOR_GREEN);
		}
	}
}
#endif // COMPILE_TOUCH_FUNCTIONS

void EXTI0_IRQHandler(){
	disableInterruptIRQ(EXTI0_IRQn);
	clearInterruptEXTI(1);

	if(ConnectFour_DropPiece(Game.currentCol)){

		ConnectFour_DrawBoard();
		if(ConnectFour_CheckWin()){
			Game.winner = Game.currentPlayer;
			if(Game.currentPlayer == PLAYER_ONE){
				Game.YellowWins++;
			}
			else{
				Game.RedWins++;
			}
			Game.state = STATE_GAME_OVER;
			ConnectFour_GameOver();
		}
		else if(ConnectFour_CheckDraw()){
			Game.isDraw = 1;
			Game.state = STATE_GAME_OVER;
			ConnectFour_GameOver();
		}
		else{
			//
			if(Game.gameMode == GAMEMODE_ONE_PLAYER && Game.currentPlayer == PLAYER_ONE){
				Game.currentPlayer = PLAYER_TWO;

				ConnectFour_ComputerMove();
				ConnectFour_DrawBoard();
				Game.currentCol = 3;
				if(ConnectFour_CheckWin()){
					Game.winner = COMPUTER_PLAYER;
					Game.RedWins++;
					Game.state = STATE_GAME_OVER;
					ConnectFour_GameOver();
				}
				else if(ConnectFour_CheckDraw()){
					Game.isDraw = 1;
					Game.state = STATE_GAME_OVER;
					ConnectFour_GameOver();
				}
				else{
					Game.currentPlayer = PLAYER_ONE;
				}
			}
			else if(Game.gameMode == GAMEMODE_TWO_PLAYER) {
				if(Game.currentPlayer == PLAYER_ONE){
					Game.currentPlayer = PLAYER_TWO;
				}
				else{
					Game.currentPlayer = PLAYER_ONE;
				}
			}
			screen2();
			ConnectFour_DrawPiece();
			ConnectFour_DrawBoard();
		}
	}
	clearInterruptIRQ(EXTI0_IRQn);
	enableInterruptIRQ(EXTI0_IRQn);
}

