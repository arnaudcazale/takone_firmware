/*
 * translateur.h
 *
 *  Created on: 29 janv. 2020
 *      Author: Samuel
 */

#ifndef TRANSLATEUR_H_
#define TRANSLATEUR_H_

#include "stm32g0xx_hal.h"


#define TYPE_HORI 1
#define TYPE_VERT 2

#define SENSE_UP 1
#define SENSE_DOWN 0

#define SENSE_LEFT 0
#define SENSE_RIGHT 1

#define SENSE_FORWARD 0
#define SENSE_BACKWARD 1

#define STATE_UP 1
#define STATE_DOWN 2
#define STATE_IN 3
#define STATE_LEFT 4
#define STATE_RIGHT 5
#define STATE_TRIGGERED 6

#define RETRACT_TIME 250000


typedef struct Translateur{
	uint8_t type ;
	uint8_t state ;
	uint8_t sense ;
	uint32_t time_change ;
} Translateur;

void updateTranslateurState(Translateur* t, uint8_t sense, uint32_t time_us) ;



#endif /* TRANSLATEUR_H_ */
