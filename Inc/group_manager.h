/*
 * group_manager.h
 *
 *  Created on: 15 janv. 2020
 *      Author: Samuel
 */

#ifndef GROUP_MANAGER_H_
#define GROUP_MANAGER_H_

#include "group_synchro.h"
#define NUMGROUPS 8
#define NUMVCAPTORS 8
#define NUMMOTORS 8
#define NUMEXTCAPTORS 2
#define NUMUART 3

typedef struct params
{
	GroupSynchro gss[NUMGROUPS] ;
	VCaptor vcs[NUMVCAPTORS] ;
	Motor motors[NUMMOTORS] ;
	ExtCaptor xcs[NUMEXTCAPTORS] ;
	UART_HandleTypeDef huarts[NUMUART] ;
}params ;

void addGroup(params* gp, uint8_t type, uint8_t gId, uint8_t* ids, uint8_t length) ;
void eraseGroup(params* gp, uint8_t gId) ;
void initGroups(params* gp) ;
uint8_t collapseGroups(params* gp, uint8_t gId1, uint8_t gId2) ;

void update(params* gp, uint32_t time_us) ;
void updateGroupSynchros(params* gp, uint32_t time_us) ;
void updateVCaptors(params* gp, uint32_t time_us) ;
void updateMotors(params* gp, uint32_t time_us) ;

#endif /* GROUP_MANAGER_H_ */
