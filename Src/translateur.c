/*
 * translateur.c
 *
 *  Created on: 29 janv. 2020
 *      Author: Samuel
 */

#include "translateur.h"

void updateTranslateurState(Translateur* t, uint8_t sense, uint32_t time_us){
	if(t->type == TYPE_HORI)
	{
		if(sense == SENSE_UP)
							{
								if(t->sense == SENSE_DOWN)
											{
													t->sense = sense ;
													t->time_change = time_us ;
											}
							}
		if(sense == SENSE_DOWN)
					{
						if(t->sense == SENSE_UP)
									{
											t->sense = sense ;
											t->time_change = time_us ;
										}
					}
		if(t->state == STATE_UP)
		{
			if(time_us - t->time_change > RETRACT_TIME )
				{
					t->state = STATE_IN ;
				}
		}
		if(t->state == STATE_DOWN)
				{
						if(time_us - t->time_change > RETRACT_TIME )
						{
							t->state = STATE_IN ;
						}
				}
		if(t->state == STATE_IN)
		{
			if(time_us - t->time_change > RETRACT_TIME )
			{
				if(t->sense == SENSE_UP)
					t->state = STATE_UP ;
				if(t->sense == SENSE_DOWN)
					t->state = STATE_DOWN ;
			}
		}
	}
	if(t->type == TYPE_VERT)
	{

	}
}

