/*
 * vcaptor.h
 *
 *  Created on: 15 janv. 2020
 *      Author: Samuel
 */

#ifndef VCAPTOR_H_
#define VCAPTOR_H_

#define NUMVCAPTORS 8
#define NUMMEASURES 5
#define NUMSPEEDS 6
#include "motor.h"

typedef struct VCaptor
{
	uint8_t id ;
	uint8_t measuring ;
	uint8_t lastMeasures[NUMMEASURES] ;

	uint8_t lastAggMeasure ;
	float speeds[NUMSPEEDS] ;
	uint8_t iSpeed ;
	uint8_t mode ;
	uint8_t calib_update ;
	uint8_t calib_start ;
	uint8_t calib_delta ;
	uint8_t calib_id ;
	float tSpeed ;
	GPIO_TypeDef* inputPinF;
	uint16_t inputPinN ;
	uint32_t nbTour ;

	uint32_t t1_i ;
	uint32_t lastNbTour ;
	uint8_t update ;
	Motor* mot ;
	uint8_t counter ;

	uint16_t target_currTour ;
	uint16_t target_tTour ;

	uint8_t proc_token ;
	uint32_t time_token ;

}VCaptor;

void updateVCaptor(VCaptor* vc, uint32_t time_us) ;
void updateVSpeed(VCaptor* vc, uint32_t time_us) ;
void standardInitVCaptor(VCaptor* vc) ;

float getAverageSpeed (VCaptor* vc) ;
void startMeasure(VCaptor* vc) ;

void startTargeting(VCaptor* vc, uint16_t tTour) ;
void reachPosition(VCaptor* vc, float speed) ;

void startResetPosition(VCaptor* vc) ;
void resetPosition(VCaptor* vc, uint32_t time_us) ;

void startAutoCalibrate(VCaptor* vc, float tSpeed, uint8_t id, uint8_t sense) ;
void autoCalibrate (VCaptor* vc) ;

void keepTSpeed(VCaptor* vc, float tSpeed) ;



#endif /* VCAPTOR_H_ */
