/*
 * InterruptControl.h
 *
 *  Created on: Feb 18, 2025
 *      Author: mattg
 */



#ifndef INTERRUPTCONTROL_H_
#define INTERRUPTCONTROL_H_


//#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#define EXTI_IRQ_0 		6

void enableInterruptIRQ(uint8_t IRQ_num);
void disableInterruptIRQ(uint8_t IRQ_num);
void setInterruptIRQ(uint8_t IRQ_num);
void clearInterruptIRQ(uint8_t IRQ_num);
void clearInterruptEXTI(uint8_t IRQ_num);


#endif /* INTERRUPTCONTROL_H_ */



