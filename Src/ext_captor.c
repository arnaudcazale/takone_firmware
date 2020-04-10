/*
 * ext_captor.c
 *
 *  Created on: 16 janv. 2020
 *      Author: Samuel
 */
#include "ext_captor.h"

void standardInitExtCaptor(ExtCaptor* xc){
	xc->measuring = 0 ;
}

void startMeasuring(ExtCaptor* xc)
{
	xc->measuring = 1 ;
}

void updateXCaptor(ExtCaptor* xc, uint32_t time_us){

	uint8_t m = 0 ;
	m = HAL_GPIO_ReadPin(xc->inputPinF, xc->inputPinN) ;

	if(xc->counter < EXT_NUMMEASURES)
		{
			xc->lastMeasures[xc->counter] = m ;
			xc->counter = xc->counter + 1 ;
		}
		else
		{
			xc->counter = 0 ;
			uint8_t agg = 0 ;
			for(int i = 0 ; i < EXT_NUMMEASURES ; i++)
			{
				agg += xc->lastMeasures[i] ;
			}
			if(agg <= 1)
			{
//				if(xc->lastAggMeasure == 1)
//					{
//						xc->nbTour += 1 ;
//						if(vc->mot->sense == SENSE_UP) {
//							vc->target_currTour += 1 ;
//						} else {
//							if(vc->target_currTour > 0)
//								vc-> target_currTour -= 1 ;
//						}
//					}
				xc->lastAggMeasure = 0 ;
			}
			if(agg >= EXT_NUMMEASURES-1)
			{
//				if(vc->lastAggMeasure == 0)
//					{
//						vc->nbTour += 1 ;
//						if(vc->mot->sense == SENSE_UP) {
//							vc->target_currTour += 1 ;
//						} else {
//							if(vc->target_currTour > 0)
//								vc-> target_currTour -= 1 ;
//						}
//					}
				xc->lastAggMeasure = 1 ;
			}
		}

}
