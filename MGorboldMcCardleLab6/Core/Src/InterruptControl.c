/*
 * InterruptControl.c
 *
 *  Created on: Feb 18, 2025
 *      Author: mattg
 */


#include "InterruptControl.h"

void enableInterruptIRQ(uint8_t IRQ_num){
	if (IRQ_num < 32){
		// *NVIC_ISER_SETENABLE = (SET << IRQ_num);
		HAL_NVIC_EnableIRQ(IRQ_num);
	}
}

void disableInterruptIRQ(uint8_t IRQ_num){
	if (IRQ_num < 32){
		// *NVIC_ISER_CLEARENABLE = (SET << IRQ_num);
		HAL_NVIC_DisableIRQ(IRQ_num);
	}
}

void setInterruptIRQ(uint8_t IRQ_num){


}

void clearInterruptIRQ(uint8_t IRQ_num){
	if (IRQ_num < 32){
		// *NVIC_ISER_CLEARPENDING = (SET << IRQ_num);
		HAL_NVIC_ClearPendingIRQ(IRQ_num);
	}

}

void clearInterruptEXTI(uint8_t IRQ_num){
	// EXTI->PR |= (SET << IRQ_num);
	__HAL_GPIO_EXTI_CLEAR_FLAG(IRQ_num);
}

