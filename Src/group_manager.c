/*
 * group_manager.c
 *
 *  Created on: 15 janv. 2020
 *      Author: Samuel
 */

#include "group_manager.h"

void initGroups(params* gp)
{
	for(int i = 0 ; i < NUMGROUPS ; i++)
	{
		GroupSynchro gs ;
		initGroup(&(gs)) ;
		gp->gss[i] = gs ;
	}
}

void update(params* gp, uint32_t time_us)
{
	updateGroupSynchros(gp, time_us) ;
	updateVCaptors(gp, time_us) ;
	updateMotors(gp, time_us) ;
}

void updateGroupSynchros(params* gp, uint32_t time_us)
{
	for(int i = 0 ; i < NUMGROUPS ; i++)
	{
		if(gp->gss[i].type != GROUP_TYPE_OFF)
			updateSynchro(&(gp->gss[i]), time_us) ;
	}
}

void updateVCaptors(params* gp, uint32_t time_us)
{
	int i = 0 ;
//	if(time_us - time_start >= 100)
//	{
		for(i = 0 ; i < NUMVCAPTORS ; i++)
		{
			if (gp->vcs[i].measuring > 0 )
					{
							updateVCaptor(&(gp->vcs[i]), time_us) ;
					}
		}
//		time_start = time_us ;
//	}
}

void updateMotors(params* gp, uint32_t time_us)
{
	int i = 0 ;
	for(i = 0 ; i < NUMMOTORS ; i++){
		updateMotor(&(gp->motors[i]), time_us) ;
	}
}

void eraseGroup(params* gp, uint8_t i)
{
		initGroup(&(gp->gss[i])) ;
}

uint8_t collapseGroups(params* gp, uint8_t gId1, uint8_t gId2)
{
	if(gp->gss[gId1].type == gp->gss[gId2].type)
	{
		if(gp->gss[gId1].type == GROUP_TYPE_CAPTOR)
		{
			for(int i = gp->gss[gId1].size ; i < gp->gss[gId1].size + gp->gss[gId2].size ; i++)
			{
				gp->gss[gId1].vcs[i] = gp->gss[gId2].vcs[i-gp->gss[gId1].size] ;
			}
			gp->gss[gId1].size += gp->gss[gId2].size ;
			eraseGroup(gp, gId2) ;
			return 1 ;
		}
		if(gp->gss[gId1].type == GROUP_TYPE_MOTOR)
		{
			//TODO
		}
	}
	return 0 ;
}

void addGroup(params* gp,  uint8_t type, uint8_t gId, uint8_t* ids, uint8_t length)
{
	gp->gss[gId].type = type ;
	gp->gss[gId].size = length ;
	gp->gss[gId].id = gId ;
	if(type == GROUP_TYPE_CAPTOR)
	{
		for(int i = 0 ; i < length ; i++)
		{
			gp->gss[gId].vcs[i] = &(gp->vcs[ids[i]]) ;
		}
	}
	if(type == GROUP_TYPE_MOTOR||GROUP_TYPE_XMOTOR)
	{
		for(int i = 0 ; i < length ; i++)
		{
			gp->gss[gId].ms[i] = &(gp->motors[ids[i]]) ;
		}
	}

}
