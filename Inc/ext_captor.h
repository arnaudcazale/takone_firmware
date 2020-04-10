/*
 * ext_captor.h
 *
 *  Created on: 16 janv. 2020
 *      Author: Samuel
 */

#ifndef EXT_CAPTOR_H_
#define EXT_CAPTOR_H_

#include "stm32g0xx_hal.h"
#define EXT_NUMMEASURES 5

typedef struct ExtCaptor
{
	uint8_t id ;
	uint8_t measuring ;
	GPIO_TypeDef* inputPinF;
	uint16_t inputPinN ;
	uint8_t lastMeasures[EXT_NUMMEASURES] ;
	uint8_t lastAggMeasure ;
	uint8_t counter ;

}ExtCaptor ;

void standardInitExtCaptor(ExtCaptor* xc) ;
void startMeasuring(ExtCaptor* xc) ;

void updateXCaptor(ExtCaptor* xc, uint32_t time_us) ;

#endif /* EXT_CAPTOR_H_ */
