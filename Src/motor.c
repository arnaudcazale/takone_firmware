/*
 * motor.c
 *
 *  Created on: 15 janv. 2020
 *      Author: Samuel
 */

#include "motor.h"

void standardInitMotor(Motor* m)
{
		m->cpower = 0 ;
		m->tpower = 0 ;
		m->runTim = 0 ;
		m->last_tick = 0 ;
		m->speedUpTime = 3000 ;
		m->last_update = 0 ;
		m->sense = 0 ;
		m->tsense = 0 ;
		m->state = 0 ;
		for(int i = 0 ; i < NUMPOWERS ; i++)
		{
			m->sPowers[i] = 60 ;
		}
		m->sPowers[2] = 85 ;
		Translateur t ;
		t.state = 0 ;
		t.sense = SENSE_UP ;
		m->trans = t ;
}

void setMotorTPower(Motor* mot, uint16_t power)
{
	mot->tpower = power ;
}
void setMotorPower(Motor* mot, uint16_t power, uint32_t time_us)
{
	__HAL_TIM_SET_COMPARE(&(mot->htim), mot->channel, ((power)*MOTPERIOD)/100);
	mot->last_update = time_us ;
	mot->cpower = power ;
}
void setMotorTSense(Motor* mot, uint8_t sense)
{
	mot->tsense = sense ;
}

void updateMotor(Motor* mot, uint32_t time_us)
{
	uint32_t tDiff = time_us - mot->last_update ;
	if(tDiff > 100000000)
	{
		mot->last_update = time_us ;
		return ;
	}
	if(tDiff >= mot->speedUpTime)
	{
		if(mot->sense != mot->tsense)
		{
			if(mot->cpower != 0)
			{
				setMotorPower(mot, mot->cpower - 1, time_us) ;
			}
			else
			{
				if(mot->tsense)
					{
						HAL_GPIO_WritePin(mot->pinFam2, mot->pinNum2, 0) ;
						HAL_GPIO_WritePin(mot->pinFam1, mot->pinNum1, 1) ;
						mot->last_update = time_us ;
					}
				else
					{
						HAL_GPIO_WritePin(mot->pinFam1, mot->pinNum1, 0) ;
						HAL_GPIO_WritePin(mot->pinFam2, mot->pinNum2, 1) ;
						mot->last_update = time_us ;
					}
				mot->sense = mot->tsense ;
			}
		}
		else
		{
			if(mot->cpower < mot->tpower)
			{
				if(mot->state == 0)
				{
					mot->state = 1 ;
					if(mot->tsense)
						{
							HAL_GPIO_WritePin(mot->pinFam2, mot->pinNum2, 0) ;
							HAL_GPIO_WritePin(mot->pinFam1, mot->pinNum1, 1) ;
							mot->last_update = time_us ;
						}
					else
						{
							HAL_GPIO_WritePin(mot->pinFam1, mot->pinNum1, 0) ;
							HAL_GPIO_WritePin(mot->pinFam2, mot->pinNum2, 1) ;
							mot->last_update = time_us ;
						}
				}
				else
					{
						setMotorPower(mot, mot->cpower +1, time_us) ;
					}
			}
			if(mot->cpower > mot->tpower)
			{
				setMotorPower(mot, mot->cpower - 1, time_us) ;
			}
			if((mot->cpower == 0) && (mot->tpower == 0))
			{
				mot->state = 0 ;
				mot->last_update = time_us ;
			}
		}
	}
}




