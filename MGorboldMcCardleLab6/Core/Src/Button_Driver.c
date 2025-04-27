/*
 * Button_Driver.c
 *
 *  Created on: Feb 11, 2025
 *      Author: mattg
 */


#include "Button_Driver.h"

void Button_Init(){
	GPIO_InitTypeDef GPIOA_BUTTON;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIOA_BUTTON.Pin = GPIO_PIN_0;
	GPIOA_BUTTON.Mode = GPIO_MODE_INPUT;
	GPIOA_BUTTON.Speed = GPIO_SPEED_FREQ_HIGH; // choose speed high
	// GPIOA_BUTTON.OPType = GPIO_OPTYPE_PUSHPULL;
	GPIOA_BUTTON.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIOA_BUTTON);
}

uint8_t Button_Press(){
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == USER_BUTTON_PRESS){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

void Button_Interrupts_Init(){
	GPIO_InitTypeDef GPIOA_BUTTON;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIOA_BUTTON.Pin = GPIO_PIN_0;
	GPIOA_BUTTON.Mode = GPIO_MODE_IT_RISING;
	GPIOA_BUTTON.Speed = GPIO_SPEED_FREQ_HIGH; // choose speed high
	// GPIOA_BUTTON.OPType = GPIO_OPTYPE_PUSHPULL;
	GPIOA_BUTTON.Pull = GPIO_NOPULL;
	// GPIOA_BUTTON.Alternate = 0;
	HAL_GPIO_Init(GPIOA, &GPIOA_BUTTON);
	enableInterruptIRQ(EXTI0_IRQn);
}
