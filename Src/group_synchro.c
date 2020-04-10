/*
 * groupSynchro.c
 *
 *  Created on: 15 janv. 2020
 *      Author: Samuel
 */

#include "group_synchro.h"
//____________________________________________GESTION DE DONNÉES_____________________________________________
void initGroup(GroupSynchro* gs)
{
		gs->type = GROUP_TYPE_OFF ;
		for(int j = 0 ; j < MAX_GROUP_LENGTH ; j++) {
			gs->vcs[j] = 0 ;
			gs->speeds[j] = 0 ;
			gs->ms[j] = 0 ;
			gs->powers[j] = 0 ;
			gs->senses[j] = 0 ;
		}

		gs->avSpeed = HIGH_AVSPEED ;
//		gs->proc_token = 0 ;
		gs->target_time = 0 ;
		gs->mr_token = 0 ;
		gs->hl_token = 0 ;
		gs->time_token = 0 ;
		gs->mode = MODE_IDLE ;
		gs->size = 0 ;
}

void updateSynchro(GroupSynchro* gs, uint32_t time_us)
{
	if(gs->type == GROUP_TYPE_CAPTOR)
	{
		if(gs->mode == MODE_CALIB)
				GS_autoCalibrate(gs) ;
		if (gs->mode == MODE_TARGET)
				GS_reachPosition(gs) ;
		if (gs->mode == MODE_TARGET_RESET)
				GS_resetPosition(gs, time_us) ;
	}

	if(gs->type == GROUP_TYPE_MOTOR)
	{

	}

	if(gs->type == GROUP_TYPE_XMOTOR)
	{

	}
}

//
void GS_startMotors(GroupSynchro* gs, uint16_t* powers, uint8_t* senses){
	if(gs->type == GROUP_TYPE_MOTOR || GROUP_TYPE_XMOTOR)
		for(int i = 0 ; i < gs->size ; i++)
		{
			setMotorTPower(gs->ms[i], powers[i]) ;
			setMotorTSense(gs->ms[i], senses[i]) ;
		}
	if(gs->type == GROUP_TYPE_CAPTOR)
		for(int i = 0 ; i < gs->size ; i++)
		{
			setMotorTPower(gs->vcs[i]->mot, powers[i]) ;
			setMotorTSense(gs->vcs[i]->mot, senses[i]) ;
		}
}

void GS_stopMotors(GroupSynchro* gs)
{
	if(gs->type == GROUP_TYPE_MOTOR || GROUP_TYPE_XMOTOR)
		for(int i = 0 ; i < gs->size ; i++)
		{
			setMotorTPower(gs->ms[i], 0) ;
		}
	if(gs->type == GROUP_TYPE_CAPTOR)
		for(int i = 0 ; i < gs->size ; i++)
		{
			setMotorTPower(gs->vcs[i]->mot, 0) ;
		}
}

//______________________________________________________ATTEINDRE POSITION______________________________________________

void GS_startTargeting(GroupSynchro* gs, float avSpeed, uint16_t tTour)
{
	if(gs->type == GROUP_TYPE_CAPTOR)
	{
		for(int i = 0 ; i < gs->size ; i++)
		{
			startTargeting(gs->vcs[i], tTour) ;
		}
		gs->avSpeed = avSpeed ;
		gs->mode = MODE_TARGET ;
	}
}

void GS_reachPosition(GroupSynchro* gs)
{
	if(gs->type == GROUP_TYPE_CAPTOR)
	{
		uint8_t barrier = 1 ;
		uint8_t update_barrier = 1 ;
//		if()
		for(int i = 0 ; i < gs->size ; i++)
				{
					if(gs->vcs[i]->mode == MODE_TARGET)
						{
							reachPosition(gs->vcs[i], gs->speeds[i]) ;
							if(gs->vcs[i]->calib_update == 0)
								update_barrier = 0 ;
							barrier = 0 ;
						}
				}
		if(barrier > 0)
			gs->mode = MODE_IDLE ;

		if(update_barrier > 0)
		{
			computeSpeeds (gs, gs->avSpeed) ;
			for(int i = 0 ; i < gs->size ; i++)
							{
								if(gs->vcs[i]->mode == MODE_TARGET) {
										keepTSpeed(gs->vcs[i], gs->speeds[i]) ;
						}
			}
		}
	}
}

//______________________________________________________RESET POSITION______________________________________________

void GS_startResetPosition(GroupSynchro* gs)
{
	if(gs->type == GROUP_TYPE_CAPTOR)
	{
		for(int i = 0 ; i < gs->size ; i++)
		{
			startResetPosition(gs->vcs[i]) ;
		}
		gs->mode = MODE_TARGET_RESET ;
	}
}

void GS_resetPosition(GroupSynchro* gs, uint32_t time_us)
{
	if(gs->type == GROUP_TYPE_CAPTOR)
	{
		uint8_t barrier = 1 ;
		for(int i = 0 ; i < gs->size ; i++)
				{
					if(gs->vcs[i]->mode == MODE_TARGET_RESET)
						{
							resetPosition(gs->vcs[i], time_us) ;
							barrier = 0 ;
						}
				}
		if(barrier)
			gs->mode = MODE_IDLE ;
	}
}

//______________________________________________________AUTOCALIBRAGE______________________________________________

void GS_startAutoCalibrate(GroupSynchro* gs, float tSpeed, uint8_t id, uint8_t sense)
{
	if(gs->type == GROUP_TYPE_CAPTOR)
	{
		for(int i = 0 ; i < gs->size ; i++)
		{
			startAutoCalibrate(gs->vcs[i], tSpeed, id ,sense) ;
		}
		gs->mode = MODE_CALIB ;
	}
}

void GS_autoCalibrate(GroupSynchro* gs)
{
	if(gs->type == GROUP_TYPE_CAPTOR)
	{
		uint8_t barrier = 1 ;
		for(int i = 0 ; i < gs->size ; i++)
				{
					if(gs->vcs[i]->mode == MODE_CALIB)
						{
							autoCalibrate(gs->vcs[i]) ;
							barrier = 0 ;
						}
				}
		if(barrier)
			gs->mode = MODE_IDLE ;
	}
}

//______________________________________________________ASSERVISSEMENT GLOBAL______________________________________________

void computeSpeeds(GroupSynchro* gs, float tSpeed)
{
	float avTour = 0 ;
	for(int i = 0 ; i < gs->size ; i++)
	{
		avTour = avTour + (float)(gs->vcs[i]->target_currTour) ;
	}
	avTour /= gs->size ;
	float k = 1.1 ;
	for(int i = 0 ; i < gs->size ; i++)
	{
		uint16_t diffTour = 0 ;

		if(gs->vcs[i]->target_currTour < gs->vcs[i]->target_tTour)
			diffTour = gs->vcs[i]->target_tTour - gs->vcs[i]->target_currTour ;
		else
			diffTour = gs->vcs[i]->target_currTour - gs->vcs[i]->target_tTour ;

//		if(diffTour > 20 && diffTour < 60)
//		{
//			if(gs->vcs[i]->mot->sense == SENSE_UP)
//				gs->vcs[i]->mot->sPowers[0] = gs->vcs[i]->mot->tpower ;
//			else
//				gs->vcs[i]->mot->sPowers[1] = gs->vcs[i]->mot->tpower ;
//		}
		float speed = tSpeed ;
//		if(diffTour < 20)
//			speed = 30 ;

		if(gs->vcs[i]->mot->sense == SENSE_UP)
			gs->speeds[i] = speed + k*(avTour - (float)(gs->vcs[i]->target_currTour)) ;
		else
			gs->speeds[i] = speed + k*((float)(gs->vcs[i]->target_currTour)- avTour) ;

		if(gs->speeds[i] < 0)
			gs->speeds[i] = 0 ;
	}
}

uint8_t GS_isOut(GroupSynchro* gs)
{
	uint8_t barrier = 0 ;
	for(int i = 0 ; i < gs->size ; i++)
	{
		if(gs->type == GROUP_TYPE_MOTOR || GROUP_TYPE_XMOTOR)
		{
			if(gs->ms[i]->trans.state != STATE_IN){
				barrier ++ ;
			}
		}
	}
	return barrier ;
}

