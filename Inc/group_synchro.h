/*
 * group_synchro.h
 *
 *  Created on: 15 janv. 2020
 *      Author: Samuel
 */

#ifndef GROUP_SYNCHRO_H_
#define GROUP_SYNCHRO_H_

#define GROUP_TYPE_OFF 0
#define GROUP_TYPE_CAPTOR 1
#define GROUP_TYPE_MOTOR 2
#define GROUP_TYPE_XMOTOR 3

#define MAX_GROUP_LENGTH 4

#define MODE_IDLE 0
#define MODE_CALIB 1
#define MODE_TARGET 10
#define MODE_TARGET_RESET 11
#define MODE_TARGET_LEFT 12

#define HIGH_AVSPEED 95.3
#define LOW_AVSPEED 30

#include "stm32g0xx_hal.h"
#include "vcaptor.h"
#include "motor.h"
#include "ext_captor.h"


typedef struct GroupSynchro
{
	uint8_t type ;
	uint8_t id;
	uint8_t size ;
	VCaptor* vcs[MAX_GROUP_LENGTH] ;
	Motor* ms[MAX_GROUP_LENGTH] ;
	ExtCaptor xc ;
	float speeds[MAX_GROUP_LENGTH] ;
	float avSpeed;

	uint16_t powers[MAX_GROUP_LENGTH] ;
	uint8_t senses[MAX_GROUP_LENGTH] ;

//	uint8_t proc_token ;
	uint8_t mr_token ;
	uint8_t hl_token ;
	uint32_t time_token ;

	uint32_t target_time ;

	uint8_t mode ;
	uint8_t MR_mode ;
	uint8_t HL_mode ;

	uint16_t MR_arg ;
	uint16_t HL_arg ;

	uint8_t state ;

}GroupSynchro;

void initGroup(GroupSynchro* gs) ;
void updateSynchro(GroupSynchro* gs, uint32_t time_us) ;

void GS_startTargeting(GroupSynchro* gs, float avSpeed, uint16_t tTour) ;
void GS_reachPosition(GroupSynchro* gs) ;

void GS_startResetPosition(GroupSynchro* gs) ;
void GS_resetPosition(GroupSynchro* gs, uint32_t time_us) ;

void GS_startAutoCalibrate(GroupSynchro* gs, float tSpeed, uint8_t id, uint8_t sense) ;
void GS_autoCalibrate(GroupSynchro* gs) ;

void GS_startMotors(GroupSynchro* gs, uint16_t* powers, uint8_t* senses) ;
void GS_stopMotors(GroupSynchro* gs) ;

void computeSpeeds(GroupSynchro* gs, float tSpeed) ;

uint8_t GS_isOut(GroupSynchro* gs) ;

#endif /* GROUP_SYNCHRO_H_ */
