/*
 * mr_manager.h
 *
 *  Created on: 15 janv. 2020
 *      Author: Samuel
 */

#ifndef MR_MANAGER_H_
#define MR_MANAGER_H_

#include "group_manager.h"

#define MR_MODE_IDLE 0
#define MR_MODE_TARGET_LEFT 10
#define MR_MODE_BLIND_MOTOR 20
#define MR_MODE_BS_MOTOR 30
#define MR_MODE_REACH_TOP 40
#define MR_MODE_REACH_BOTTOM 41
#define MR_MODE_GO_IN 50

#define GO_IN_POWER 70
#define REACH_TOP_LEVEL 370
#define REACH_TOP_DELAY 1200000
#define REACH_TOP_POWER 50

#define REACH_BOTTOM_LEVEL 20
#define REACH_BOTTOM_DELAY 1000000
#define REACH_BOTTOM_POWER 40

#define ESCAMOTING_TIME 250000

#define TOP_LANDING_LEVEL 330

#define TOP_PASSAGE_LEVEL 379
#define MIDDLE_PASSAGE_LEVEL 260
#define LOW_PASSAGE_LEVEL 130


void MR_initGroups(params* p) ;
void MR_update(params* p, uint32_t time_us) ;
void MR_addGroup(params* gp,  uint8_t type, uint8_t gId, uint8_t* ids, uint8_t length) ;
void MR_eraseGroup(params* gp, uint8_t gId) ;

void MR_targetLeft(params* gp, GroupSynchro* gs) ;
void MR_startTargetLeft(params* gp, GroupSynchro* gs, uint16_t level) ;

void MR_startBlindMotion(GroupSynchro* gs, uint8_t* powers, uint8_t* senses, uint32_t time);
void MR_blindMotorMotion(GroupSynchro* gs, uint32_t time_us) ;

void MR_startBackStopMotion(GroupSynchro* gs, uint16_t* powers, uint8_t* senses);
void MR_backStopMotion(GroupSynchro* gs) ;

void MR_startReachTop(GroupSynchro* gs) ;
void MR_reachTop(GroupSynchro* gs) ;

void MR_startReachBottom(GroupSynchro* gs) ;
void MR_reachBottom(GroupSynchro* gs) ;

void MR_goHori(GroupSynchro* gs, uint8_t k, uint8_t sense, uint32_t time, uint8_t invert) ;

void MR_startGoIn(GroupSynchro* gs) ;
void MR_goIn(GroupSynchro* gs, uint32_t time_us) ;

#endif /* MR_MANAGER_H_ */
