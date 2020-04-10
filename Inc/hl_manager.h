/*
 * hl_go.h
 *
 *  Created on: 15 janv. 2020
 *      Author: Samuel
 */

#ifndef HL_MANAGER_H_
#define HL_MANAGER_H_

#define HL_MODE_IDLE 0
#define HL_MODE_GO_UP 1
#define HL_MODE_GO_DOWN 2

#define HL_MODE_GET_FROM_DOWN 10
#define HL_MODE_GET_FROM_UP 11

#define HL_MODE_GO_RIGHT 20
#define HL_MODE_GO_LEFT 21

#define HL_MODE_GO_FORWARD 22
#define HL_MODE_GO_BACKWARD 23

#define HL_MODE_GET_FROM_RIGHT 30
#define HL_MODE_GET_FROM_LEFT 31
#define HL_MODE_GET_FROM_FORWARD 32
#define HL_MODE_GET_FROM_BACKWARD 33

#define HL_MODE_BS_MOTION 50

#define LONG_DELAY 10000000
#define SHORT_DELAY 3500000
#define ACCEL_DELAY 1500000

#define COM_A 'a'
#define COM_B 'b'
#define COM_C 'c'
#define COM_D 'd'
#define COM_1 '1'
#define COM_2 '2'
#define COM_3 '3'
#define COM_4 '4'
#define COM_5 '5'
#define COM_6 '6'
#define COM_7 '7'
#define COM_8 '8'
#define COM_9 '9'
#define COM_10 'l'


#include "mr_manager.h"

void HL_initGroups(params* p) ;
void HL_update(params* p, uint32_t time_us) ;
void HL_eraseGroup(params* p, uint8_t gId) ;

void HL_StartGoUp(GroupSynchro* gs) ;
void HL_GoUp(params* gp, GroupSynchro* gs) ;
void HL_StartGetFromDown(GroupSynchro* gs) ;
void HL_GetFromDown(params* p, GroupSynchro* gs) ;

void HL_StartGoDown(GroupSynchro* gs) ;
void HL_GoDown(params* gp, GroupSynchro* gs) ;
void HL_StartGetFromUp(GroupSynchro* gs) ;
void HL_GetFromUp(params* p, GroupSynchro* gs) ;

void HL_StartGoRight(GroupSynchro* gs) ;
void HL_GoRight(params* gp, GroupSynchro* gs) ;
void HL_StartGetFromLeft(GroupSynchro* gs) ;
void HL_GetFromLeft(params* p, GroupSynchro* gs) ;

void HL_StartGoLeft(GroupSynchro* gs) ;
void HL_GoLeft(params* gp, GroupSynchro* gs) ;
void HL_StartGetFromRight(GroupSynchro* gs) ;
void HL_GetFromRight(params* p, GroupSynchro* gs) ;

void HL_StartGoForward(GroupSynchro* gs) ;
void HL_GoForward(params* gp, GroupSynchro* gs) ;
void HL_StartGetFromBackward(GroupSynchro* gs) ;
void HL_GetFromBackward(params* p, GroupSynchro* gs) ;

void HL_StartGoBackward(GroupSynchro* gs) ;
void HL_GoBackward(params* gp, GroupSynchro* gs) ;
void HL_StartGetFromForward(GroupSynchro* gs) ;
void HL_GetFromForward(params* p, GroupSynchro* gs) ;

void HL_startGoBackStop(params* gp, GroupSynchro* gs, uint8_t sense) ;
void HL_goBackStop( GroupSynchro* gs) ;

void HL_handle_message(params* p, uint8_t* buf) ;


#endif /* HL_GO_H_ */
