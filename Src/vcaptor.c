/*
 * vcaptor.c
 *
 *  Created on: 15 janv. 2020
 *      Author: Samuel
 */


#include "vcaptor.h"

#define CAPTOR_MODE_IDLE 0

#define CAPTOR_MODE_CALIB 1

#define CAPTOR_MODE_TARGET 10
#define CAPTOR_MODE_TARGET_RESET 11

void updateVCaptor(VCaptor* vc, uint32_t time_us)
{
	uint8_t m = 0 ;
	m = HAL_GPIO_ReadPin(vc->inputPinF, vc->inputPinN) ;

	if(vc->mot->tpower != vc->mot->cpower)
		vc->counter = 0 ;

	if(vc->counter < NUMMEASURES)
	{
		vc->lastMeasures[vc->counter] = m ;
		vc->counter = vc->counter + 1 ;
	}
	else
	{
		vc->counter = 0 ;
		uint8_t agg = 0 ;
		for(int i = 0 ; i < NUMMEASURES ; i++)
		{
			agg += vc->lastMeasures[i] ;
		}
		if(agg <= 1)
		{
			if(vc->lastAggMeasure == 1)
				{
					vc->nbTour += 1 ;
					if(vc->mot->sense == SENSE_UP) {
						vc->target_currTour += 1 ;
					} else {
						if(vc->target_currTour > 0)
							vc-> target_currTour -= 1 ;
					}
				}
			vc->lastAggMeasure = 0 ;
		}
		if(agg >= NUMMEASURES-1)
		{
			if(vc->lastAggMeasure == 0)
				{
					vc->nbTour += 1 ;
					if(vc->mot->sense == SENSE_UP) {
						vc->target_currTour += 1 ;
					} else {
						if(vc->target_currTour > 0)
							vc-> target_currTour -= 1 ;
					}
				}
			vc->lastAggMeasure = 1 ;
		}
	}
	if(( vc->nbTour - vc->lastNbTour >= 4))
		{
			updateVSpeed(vc, time_us) ;
			vc->t1_i = time_us ;
			vc->lastNbTour = vc->nbTour;
		}
}
void updateVSpeed(VCaptor* vc, uint32_t time_us)
{
	vc->speeds[vc->iSpeed] = ((float)(vc->nbTour - vc->lastNbTour)/(float)(time_us-vc->t1_i))*1000000 ;
	if(vc->iSpeed < NUMSPEEDS-1)
		vc->iSpeed ++ ;
	else
		{
			vc->iSpeed = 0 ;
			vc->calib_update = 1 ;
		}
}
float getAverageSpeed (VCaptor* vc)
{
	float av = 0 ;
	for(int i = 0 ; i < NUMSPEEDS ; i++)
	{
		av += vc->speeds[i] ;
	}
	av /= NUMSPEEDS ;
	return av ;
}

void resetSpeedData(VCaptor* vc)
{
	for(int i = 0 ; i < NUMSPEEDS ; i++)
	{
		vc->speeds[i] = 0 ;
	}
	vc->iSpeed = 0 ;
	vc->calib_update = 0 ;

}

void standardInitVCaptor(VCaptor* vc)
{
	vc->measuring = 0 ;

	vc->nbTour = 0 ;
	vc->lastNbTour = 0 ;
	for(int i = 0 ; i < NUMMEASURES ; i++){
		vc->lastMeasures[i] = 0 ;
	}
	for(int i = 0 ; i < NUMSPEEDS ; i++) {
		vc->speeds[i] = 0 ;
	}
	vc->iSpeed = 0 ;
	vc->t1_i = 0 ;
	vc->update = 0;
	vc->counter = 0 ;
	vc->lastAggMeasure = 0 ;

	vc->calib_update = 0 ;
	vc->calib_start = 0 ;
	vc->calib_delta = 28 ;
	vc->calib_id = 0 ;
	vc->tSpeed = 0 ;
	vc->mode = CAPTOR_MODE_IDLE ;

	vc->target_tTour = 0 ;
	vc->target_currTour = 0 ;

	vc->proc_token = 0 ;
	vc->time_token = 0 ;
}

void startMeasure(VCaptor* vc)
{
	if(vc->measuring == 0)
		vc->measuring = 1 ;
}

void startTargeting(VCaptor* vc, uint16_t tTour)
{
	vc->target_tTour = tTour ;
	vc->mode = CAPTOR_MODE_TARGET ;
	if(vc->target_currTour > vc->target_tTour )
	{
		setMotorTSense(vc->mot, SENSE_DOWN) ;
		setMotorTPower(vc->mot, vc->mot->sPowers[1]) ;
	}
	else if (vc->target_currTour < vc->target_tTour )
	{
		setMotorTSense(vc->mot, SENSE_UP) ;
		setMotorTPower(vc->mot, vc->mot->sPowers[0]) ;
	}
}

void reachPosition(VCaptor* vc, float speed)
{

	if((vc->target_currTour >= vc->target_tTour && vc->mot->tsense == SENSE_UP)||(vc->target_currTour <= vc->target_tTour && vc->mot->tsense == SENSE_DOWN))
	{
		setMotorTPower(vc->mot, 0) ;
		vc->mode = CAPTOR_MODE_IDLE ;
		resetSpeedData(vc) ;
	}
}

void startResetPosition(VCaptor* vc)
{
	vc->proc_token = 0 ;
	vc->mode = CAPTOR_MODE_TARGET_RESET ;
}

void resetPosition(VCaptor* vc, uint32_t time_us)
{
	if(vc->proc_token == 0)
		{
			setMotorTPower(vc->mot, 100) ;
			setMotorTSense(vc->mot, SENSE_DOWN) ;
			vc->proc_token = 1 ;
			vc->time_token = time_us ;
		}
	if(vc->proc_token == 1 && time_us - vc->time_token >= 5000000)
	{
		setMotorTPower(vc->mot, 0) ;
		vc->proc_token = 0 ;
		vc->target_currTour = 0 ;
		resetSpeedData(vc) ;
		vc->mode = CAPTOR_MODE_IDLE ;
		vc->mot->trans.state = STATE_LEFT ;
	}
}

void startAutoCalibrate(VCaptor* vc, float tSpeed, uint8_t id, uint8_t sense)
{
	vc-> calib_delta = 14 ;
	vc-> calib_start = 72 ;
	Motor* m = vc->mot ;
	setMotorTPower(m, vc->calib_start) ;
	setMotorTSense(m, sense) ;
	vc->iSpeed = 0 ;
	vc->tSpeed = tSpeed ;
	vc->calib_id = id ;
	vc->calib_update = 0 ;
	vc->mode = CAPTOR_MODE_CALIB ;
}

void autoCalibrate (VCaptor* vc)
{
	if(vc->calib_delta > 0 )
	{
		if(vc->calib_update > 0)
		{
			Motor* m = vc->mot ;
			float cSpeed = getAverageSpeed(vc) ;
			if (cSpeed > vc->tSpeed)
				{
					m->sPowers[vc->calib_id] = vc->calib_start - vc->calib_delta;
					vc->calib_start = vc->calib_start - vc->calib_delta ;
				}
			else
				{
					m->sPowers[vc->calib_id] = vc->calib_start + vc->calib_delta ;
					vc->calib_start = vc->calib_start + vc->calib_delta ;
				}
			vc->calib_delta = vc->calib_delta >> 1 ;
			setMotorTPower(m, m->sPowers[vc->calib_id]) ;
			vc->calib_update = 0 ;
			if(vc->calib_delta == 0)
				{
					vc->mode = CAPTOR_MODE_IDLE ;
					resetSpeedData(vc) ;
					//flash_page_erase(62) ;
//					flash_write(0x0801F060, (uint64_t)m->sPowers[id]) ;
					//uint64_t l = flash_read(0x0801F060) ;
					//l= l ;
					setMotorTPower(vc->mot, 0) ;
				}
		}
	}
}

void keepTSpeed(VCaptor* vc, float tSpeed)
{
	if(vc->calib_update > 0)
	{
		float cSpeed = getAverageSpeed(vc) ;
		Motor* m = vc->mot ;
		float k = 0.3 ;
		uint16_t power = m->tpower ;
		if(cSpeed < tSpeed)
			power += (uint16_t)(k*(tSpeed-cSpeed)) ;
		if(power > 100)
			power = 100 ;
		if(cSpeed > tSpeed)
			{
			if(((uint16_t)(k*(cSpeed-tSpeed)))> power)
					power = 0 ;
			else power -= (uint16_t)(k*(cSpeed-tSpeed)) ;
			}
		setMotorTPower(m, power) ;
		vc->calib_update = 0 ;
	}
}
