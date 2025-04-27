/*
 * Button_Driver.h
 *
 *  Created on: Feb 11, 2025
 *      Author: mattg
 */

#ifndef BUTTON_DRIVER_H_
#define BUTTON_DRIVER_H_

#include "InterruptControl.h"

#define TRUE 	1
#define FALSE	0

#define USER_BUTTON_PORT		GPIOA	 // User button PA0 (GPIOA 0)
#define USER_BUTTON_PIN_NUM		0x0
#define USER_BUTTON_PRESS		0x1
#define USER_BUTTON_NOTPRESS	0x0

void Button_Init();
uint8_t Button_Press();
void Button_Interrupts_Init();


#endif /* BUTTON_DRIVER_H_ */
