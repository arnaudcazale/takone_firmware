/*
 * mr_manager.c
 *
 *  Created on: 15 janv. 2020
 *      Author: Samuel
 */
#include "mr_manager.h"

void MR_initGroups(params* gp){
	initGroups(gp) ;
	for(int i = 0 ; i < NUMGROUPS ; i++)
	{
		gp->gss[i].MR_mode = MR_MODE_IDLE ;
		gp->gss[i].MR_arg = 0 ;
	}
}

void MR_addGroup(params* gp,  uint8_t type, uint8_t gId, uint8_t* ids, uint8_t length)
{
	gp->gss[gId].MR_mode = MR_MODE_IDLE ;
	gp->gss[gId].MR_arg = 0 ;
	addGroup(gp, type, gId, ids, length) ;
}

void MR_eraseGroup(params* gp, uint8_t gId)
{
	gp->gss[gId].MR_mode = MR_MODE_IDLE ;
	gp->gss[gId].MR_arg = 0 ;
	eraseGroup(gp, gId) ;
}

void MR_update(params* gp, uint32_t time_us)
{
	for(int i = 0 ; i < NUMGROUPS ; i++)
	{
		if(gp->gss[i].type == GROUP_TYPE_CAPTOR)
		{
					if(gp->gss[i].MR_mode == MR_MODE_TARGET_LEFT)
						MR_targetLeft(gp, &(gp->gss[i])) ;
					if(gp->gss[i].MR_mode == MR_MODE_BLIND_MOTOR)
							MR_blindMotorMotion(&(gp->gss[i]), time_us) ;
					if(gp->gss[i].MR_mode == MR_MODE_REACH_TOP)
							MR_reachTop(&(gp->gss[i])) ;
					if(gp->gss[i].MR_mode == MR_MODE_REACH_BOTTOM)
							MR_reachBottom(&(gp->gss[i])) ;
		}
		if(gp->gss[i].type == GROUP_TYPE_MOTOR)
		{
			if(gp->gss[i].MR_mode == MR_MODE_BLIND_MOTOR)
					MR_blindMotorMotion(&(gp->gss[i]), time_us) ;
			if(gp->gss[i].MR_mode == MR_MODE_GO_IN)
							MR_goIn(&(gp->gss[i]), time_us) ;
		}
		if(gp->gss[i].type == GROUP_TYPE_XMOTOR)
		{
			if(gp->gss[i].MR_mode == MR_MODE_BS_MOTOR)
								MR_backStopMotion(&(gp->gss[i])) ;
		}
	}
	update(gp, time_us) ;
}
//________________________________________________________________TARGET LEFT_____________________________________________________

void MR_startTargetLeft(params* gp, GroupSynchro* gs, uint16_t level)
{
	if(gs->type == GROUP_TYPE_CAPTOR)
	{
		uint8_t length = 0 ;
		for(int i = 0 ; i < gs->size ; i++)
		{
			if(gs->vcs[i]->target_currTour < level)
				length++ ;
		}
		if(length > 0 && length < gs->size)
		{
			uint8_t idsLow[length] ;
			uint8_t idsTop[gs->size - length] ;
			int j1 = 0 ;
			int j2 = 0 ;
			for(int i = 0 ; i < gs->size ; i++)
			{
				if(gs->vcs[i]->target_currTour < level)
					{
						idsTop[j1] = gs->vcs[i]->id;
						j1++ ;
					}
				else
				{
					idsLow[j2] = gs->vcs[i]->id ;
					j2++ ;
				}
			}
			uint8_t id = gs->id ;
			addGroup(gp, GROUP_TYPE_CAPTOR, id, idsLow, gs->size - length) ;
			addGroup(gp, GROUP_TYPE_CAPTOR, id+1, idsTop, length) ;

			GS_startTargeting(&(gp->gss[id]), HIGH_AVSPEED, level) ;

			if(level < LOW_PASSAGE_LEVEL)
				GS_startTargeting(&(gp->gss[id+1]),HIGH_AVSPEED, LOW_PASSAGE_LEVEL) ;
			else if(level < MIDDLE_PASSAGE_LEVEL)
				GS_startTargeting(&(gp->gss[id+1]),HIGH_AVSPEED, MIDDLE_PASSAGE_LEVEL) ;
			else if (level < TOP_PASSAGE_LEVEL)
				GS_startTargeting(&(gp->gss[id+1]),HIGH_AVSPEED, TOP_PASSAGE_LEVEL) ;
			gs->mr_token = 1 ;
		}
		else if(length == 0 )
		{
			GS_startTargeting(gs,HIGH_AVSPEED, level) ;
			gs->mr_token = 3 ;
		}
		else if (length == gs->size )
		{
			if(level < LOW_PASSAGE_LEVEL)
				GS_startTargeting(gs,HIGH_AVSPEED, LOW_PASSAGE_LEVEL) ;
			else if(level < MIDDLE_PASSAGE_LEVEL)
				GS_startTargeting(gs,HIGH_AVSPEED, MIDDLE_PASSAGE_LEVEL) ;
			else if (level < TOP_PASSAGE_LEVEL)
				GS_startTargeting(gs,HIGH_AVSPEED, TOP_PASSAGE_LEVEL) ;

			gs->mr_token = 4 ;
		}
		gs->MR_mode = MR_MODE_TARGET_LEFT ;
		gs->MR_arg = level ;
	}
}

void MR_targetLeft(params* gp, GroupSynchro* gs)
{
	if(gs->mr_token == 1 && gp->gss[gs->id+1].mode == MODE_IDLE)
	{
		GS_startTargeting(&(gp->gss[gs->id+1]), HIGH_AVSPEED, gs->MR_arg) ;
		gs->mr_token = 2 ;
	}
	if(gs->mr_token == 2 && gp->gss[gs->id+1].mode == MODE_IDLE)
	{
		collapseGroups(gp, gs->id, gs->id+1) ;
		gs->MR_arg = 0 ;
		gs->mr_token = 0 ;
		for(int i = 0 ; i < gs->size ; i++)
		{
			gs->vcs[i]->mot->trans.state = STATE_LEFT ;
		}
		gs->MR_mode = MR_MODE_IDLE ;
	}
	if(gs->mr_token == 3 && gs->mode == MODE_IDLE )
	{
		gs->mr_token = 0 ;
		gs->MR_arg = 0 ;
		for(int i = 0 ; i < gs->size ; i++)
		{
			gs->vcs[i]->mot->trans.state = STATE_LEFT ;
		}
		gs->MR_mode = MR_MODE_IDLE ;
	}
	if(gs->mr_token == 4 && gs->mode== MODE_IDLE)
	{
		GS_startTargeting(gs,HIGH_AVSPEED, gs->MR_arg) ;
		gs->mr_token = 3 ;
	}
}
//_______________________________________________________REACH TOP__________________________________________________
void MR_startReachTop(GroupSynchro* gs)
{
	if(gs->type == GROUP_TYPE_CAPTOR)
	{
		gs->MR_mode = MR_MODE_REACH_TOP ;
		gs->MR_arg = 0 ;
		gs->mr_token = 1 ;
	}
}

void MR_reachTop(GroupSynchro* gs)
{
	if(gs->mr_token == 1)
	{
		GS_startTargeting(gs,HIGH_AVSPEED, REACH_TOP_LEVEL) ;
		gs->mr_token = 2 ;
	}
	if(gs->mr_token == 2 && gs->mode == MODE_IDLE)
	{
		uint8_t powers[gs->size] ;
		uint8_t senses[gs->size] ;
		for(int i = 0 ; i < gs->size ; i++)
		{
			powers[i] = REACH_TOP_POWER ;
			senses[i] = SENSE_UP ;
		}
		MR_startBlindMotion(gs, powers, senses, REACH_TOP_DELAY) ;
	}
}

//_______________________________________________________REACH TOP__________________________________________________
void MR_startReachBottom(GroupSynchro* gs)
{
	if(gs->type == GROUP_TYPE_CAPTOR)
	{
		gs->MR_mode = MR_MODE_REACH_BOTTOM ;
		gs->MR_arg = 0 ;
		gs->mr_token = 1 ;
	}
}

void MR_reachBottom(GroupSynchro* gs)
{
	if(gs->mr_token == 1)
	{
		GS_startTargeting(gs, HIGH_AVSPEED,REACH_BOTTOM_LEVEL) ;
		gs->mr_token = 2 ;
	}
	if(gs->mr_token == 2 && gs->mode == MODE_IDLE)
	{
		uint8_t powers[gs->size] ;
		uint8_t senses[gs->size] ;
		for(int i = 0 ; i < gs->size ; i++)
		{
			powers[i] = REACH_BOTTOM_POWER ;
			senses[i] = SENSE_DOWN ;
		}
		MR_startBlindMotion(gs, powers, senses, REACH_BOTTOM_DELAY) ;
	}
}

//_____________________________________________________HORIZONTAL MOTION__________________________________________________


void MR_goHori(GroupSynchro* gs, uint8_t k, uint8_t sense, uint32_t time, uint8_t invert)
{
			uint8_t powers[gs->size] ;
			uint8_t senses[gs->size] ;
			for(int i = 0 ; i < gs->size ; i++)
			{
				powers[i] = gs->ms[i]->sPowers[2]-k ;
				if(invert)
				{
					if(i&1)
						senses[i] = 1- sense ;
					else
						senses[i] = sense ;
				}
				else
					senses[i] = sense ;
			}
			if(gs->type == GROUP_TYPE_MOTOR)
				MR_startBlindMotion(gs, powers, senses, time);
			if(gs->type == GROUP_TYPE_XMOTOR)
				{
				 //TODO
				}
}

//______________________________________________________BLIND MOTOR MOTION______________________________________________


void MR_startBlindMotion(GroupSynchro* gs, uint8_t* powers, uint8_t* senses, uint32_t time)
{
	for(int i = 0 ; i < gs->size ; i++)
	{
		gs->powers[i] = powers[i] ;
		gs->senses[i] = senses[i] ;
	}
	gs->mr_token = 1 ;
	gs->target_time = time ;
	gs->MR_mode = MR_MODE_BLIND_MOTOR ;
}

void MR_blindMotorMotion(GroupSynchro* gs, uint32_t time_us)
{

	if(gs->mr_token == 1)
	{
		GS_startMotors(gs, gs->powers, gs->senses) ;
		gs->time_token = time_us ;
		gs->mr_token = 2 ;
	}
	if(gs->mr_token == 2 && time_us- gs->time_token > gs->target_time)
	{
		GS_stopMotors(gs) ;
		gs->MR_mode = MR_MODE_IDLE ;
		gs->mr_token = 0 ;
	}
}

//_______________________________________________________BACKSTOP MOTION__________________________

void MR_startBackStopMotion(GroupSynchro* gs, uint16_t* powers, uint8_t* senses){
	gs->mr_token = 0 ;
	for(int i = 0 ; i < gs->size ; i++)
	{
		gs->powers[i] = powers[i] ;
		gs->senses[i] = senses[i] ;
	}
	gs->mr_token = 0 ;
	gs->MR_mode = MR_MODE_BS_MOTOR ;
}

void MR_backStopMotion(GroupSynchro* gs)
{
	if(gs->mr_token == 0)
	{
		GS_startMotors(gs, gs->powers, gs->senses) ;
		gs->mr_token = 1 ;
	}
	if(gs->mr_token == 1 && gs->xc.lastAggMeasure == 1)
	{
		GS_stopMotors(gs);
		gs->mr_token = 0 ;
		gs->MR_mode = MR_MODE_IDLE ;
	}
}

//_______________________________________________GO IN METHODS______________________________________
void MR_startGoIn(GroupSynchro* gs)
{
		gs->mr_token = 0 ;
		gs->MR_mode = MR_MODE_GO_IN ;
		gs->MR_arg = 0 ;
		for(int i = 0 ; i < gs->size ; i++)
			{
				gs->powers[i] = GO_IN_POWER ;
				gs->senses[i] = SENSE_UP ;
			}
}

void MR_goIn(GroupSynchro* gs, uint32_t time_us)
{
	if(gs->mr_token == 0)
	{
		GS_startMotors(gs, gs->powers, gs->senses) ;
		gs->time_token = time_us ;
		gs->mr_token = 1 ;
	}
	if(gs->mr_token == 1 && time_us-gs->time_token >= 3*RETRACT_TIME)
	{
		for(int i = 0 ; i < gs->size ; i++)
		{
			gs->senses[i] = SENSE_DOWN ;
		}
		GS_stopMotors(gs) ;
		GS_startMotors(gs, gs->powers, gs->senses);
		gs->time_token = time_us ;
		gs->mr_token = 2 ;
	}
	if(gs->mr_token == 2 && time_us - gs->time_token >= 2*RETRACT_TIME)
	{
		GS_stopMotors(gs);
		for(int i = 0 ; i < gs->size ; i++)
		{
			if(gs->type == GROUP_TYPE_MOTOR || GROUP_TYPE_XMOTOR)
			gs->ms[i]->trans.state = STATE_IN ;
		}
		gs->mr_token = 0 ;
		gs->MR_mode = MR_MODE_IDLE ;
	}
}
