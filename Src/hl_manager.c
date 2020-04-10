/*
 * hl_manager.c
 *
 *  Created on: 15 janv. 2020
 *      Author: Samuel
 */
#include "hl_manager.h"



void HL_initGroups(params* gp){

	MR_initGroups(gp) ;
	for(int i = 0 ; i < NUMGROUPS ; i++)
	{
		gp->gss[i].HL_mode = HL_MODE_IDLE ;
		gp->gss[i].HL_arg = 0 ;
		gp->gss[i].hl_token = 0 ;
	}
}

void HL_update(params* gp, uint32_t time_us){
	for(int i = 0 ; i < NUMGROUPS ; i++)
	{
		if(gp->gss[i].type == GROUP_TYPE_CAPTOR)
		{
			if(gp->gss[i].HL_mode == HL_MODE_GO_UP)
				HL_GoUp(gp, &(gp->gss[i])) ;
			if(gp->gss[i].HL_mode == HL_MODE_GET_FROM_UP)
				HL_GetFromUp(gp, &(gp->gss[i])) ;

			if(gp->gss[i].HL_mode == HL_MODE_GO_DOWN)
				HL_GoDown(gp, &(gp->gss[i])) ;
			if(gp->gss[i].HL_mode == HL_MODE_GET_FROM_DOWN)
				HL_GetFromDown(gp, &(gp->gss[i])) ;
		}

		if(gp->gss[i].type == GROUP_TYPE_MOTOR || gp->gss[i].type == GROUP_TYPE_XMOTOR)
		{
					if(gp->gss[i].HL_mode == HL_MODE_GO_LEFT)
						HL_GoLeft(gp, &(gp->gss[i])) ;
					if(gp->gss[i].HL_mode == HL_MODE_GET_FROM_LEFT)
						HL_GetFromLeft(gp, &(gp->gss[i])) ;

					if(gp->gss[i].HL_mode == HL_MODE_GO_RIGHT)
						HL_GoRight(gp, &(gp->gss[i])) ;
					if(gp->gss[i].HL_mode == HL_MODE_GET_FROM_RIGHT)
						HL_GetFromRight(gp, &(gp->gss[i])) ;

					if(gp->gss[i].HL_mode == HL_MODE_GO_FORWARD)
						HL_GoForward(gp, &(gp->gss[i])) ;
					if(gp->gss[i].HL_mode == HL_MODE_GET_FROM_FORWARD)
						HL_GetFromForward(gp, &(gp->gss[i])) ;

					if(gp->gss[i].HL_mode == HL_MODE_GO_BACKWARD)
						HL_GoBackward(gp, &(gp->gss[i])) ;
					if(gp->gss[i].HL_mode == HL_MODE_GET_FROM_BACKWARD)
						HL_GetFromBackward(gp, &(gp->gss[i])) ;
		}
	}
	MR_update(gp, time_us) ;
}
void HL_eraseGroup(params* gp, uint8_t gId)
{
	gp->gss[gId].HL_mode = HL_MODE_IDLE ;
	MR_eraseGroup(gp, gId) ;
}

//_______________________________________________________________GO UP___________________________________________________________

void HL_StartGoUp(GroupSynchro* gs)
{
	if(gs->type == GROUP_TYPE_CAPTOR)
		{
			gs->hl_token = 1 ;
			gs->HL_mode = HL_MODE_GO_UP ;
		}
}

//
void HL_GoUp(params* gp, GroupSynchro* gs)
{
		if(gs->hl_token == 1 && gs->mode == MODE_IDLE)
		{
			uint8_t barrier = 0 ;
			for(int i = 0 ; i < gs->size ; i++)
			{
				if(gs->vcs[i]->mot->trans.state != STATE_IN)
					barrier = 1 ;
			}
			if(barrier == 1)
				MR_startTargetLeft(gp, gs, 21) ;

			uint8_t tx1[] = {COM_1} ;
			HAL_UART_Transmit(&(gp->huarts[2]), tx1, 1, 1000) ;
			gs->hl_token = 2 ;
		}
		if(gs->hl_token == 2 && gs->MR_mode == MR_MODE_IDLE)
		{
			MR_startReachTop(gs) ;
			gs->hl_token = 3 ;
		}
		if(gs->hl_token == 3 && gs->MR_mode == MR_MODE_IDLE)
		{
			for(int i = 0 ; i < gs->size ; i++)
			{
				gs->vcs[i]->mot->trans.state = STATE_IN ;
			}
			uint16_t powers[gs->size] ;
			uint8_t senses[gs->size] ;
			for(int i = 0 ; i < gs->size ; i++)
			{
				powers[i] = 50 ;
				senses[i] = SENSE_UP ;
			}
			GS_startMotors(gs, powers, senses) ;
			gs->HL_arg = 0 ;
			uint8_t tx1[] = {COM_A} ;
			HAL_UART_Transmit(&(gp->huarts[2]), tx1, 1, 1000) ;
			gs->hl_token = 4 ;
		}

		if(gs->hl_token == 4 && gs->mode == MODE_IDLE && gs->HL_arg == 1)
		{
			GS_stopMotors(gs) ;
			GS_startTargeting(gs, HIGH_AVSPEED, 340) ;
			gs->HL_arg = 0 ;
			gs->hl_token = 5 ;
		}

		if(gs->hl_token == 5 && gs->mode == MODE_IDLE)
		{
			for(int i = 0 ; i < gs->size ; i++)
			{
				gs->vcs[i]->mot->trans.state = STATE_LEFT ;
			}
			uint8_t tx1[] = {COM_C} ;
			HAL_UART_Transmit(&(gp->huarts[2]), tx1, 1, 1000) ;
			gs->hl_token = 0 ;
			gs->HL_arg = 0 ;
			gs->HL_mode = HL_MODE_IDLE ;
		}
}

void HL_StartGetFromDown(GroupSynchro* gs)
{
	if(gs->type == GROUP_TYPE_CAPTOR)
		{
			gs->hl_token = 1 ;
			gs->HL_mode = HL_MODE_GET_FROM_DOWN ;
		}
}

void HL_GetFromDown(params* gp, GroupSynchro* gs)
{
	if(gs->hl_token == 1 && gs->mode == MODE_IDLE)
	{
		GS_startTargeting(gs,HIGH_AVSPEED, 6) ;
		if(GS_isOut(&(gp->gss[0])))
			MR_startGoIn(&(gp->gss[0])) ;
//		if(GS_isOut(&(gp->gss[2])))
//			MR_startGoIn(&(gp->gss[2])) ;

		gs->hl_token = 2 ;
	}
	if(gs->hl_token == 2 && gs->mode == MODE_IDLE && gp->gss[0].MR_mode == MR_MODE_IDLE)
		{

			GS_startTargeting(gs,HIGH_AVSPEED, 1) ;
			gs->hl_token = 3 ;
		}
	if(gs->hl_token == 3 && gs->HL_arg == 2)
	{
		for(int i = 0 ; i < gs->size ; i++)
			{
				gs->vcs[i]->mot->trans.state = STATE_LEFT ;
			}
		GS_startTargeting(gs,HIGH_AVSPEED, 25) ;
		gs->HL_arg = 0 ;
		gs->hl_token = 4 ;
	}

	if(gs->hl_token == 4 && gs->mode == MODE_IDLE)
	{

		uint8_t tx1[] = {COM_A} ;
		HAL_UART_Transmit(&(gp->huarts[0]), tx1, 1, 1000) ;
		gs->hl_token = 5 ;
	}

	if(gs->hl_token == 5 && gs->HL_arg == 3)
	{
		GS_startTargeting(gs,HIGH_AVSPEED, 350) ;
		gs->HL_arg = 0 ;
		gs->hl_token = 6 ;
	}
	if(gs->hl_token == 6 && gs->mode == MODE_IDLE)
	{
		for(int i = 0 ; i < gs->size ; i++)
		{
			gs->vcs[i]->mot->trans.state = STATE_IN ;
		}
		gs->hl_token = 0 ;
		gs->HL_mode = HL_MODE_IDLE ;
		gs->HL_arg = 0 ;
	}
}

//_____________________________________________________GO DOWN___________________________________________


void HL_StartGoDown(GroupSynchro* gs)
{
	if(gs->type == GROUP_TYPE_CAPTOR)
		{
			gs->hl_token = 1 ;
			gs->HL_mode = HL_MODE_GO_DOWN ;
			gs->HL_arg = 0 ;
		}
}
//
void HL_GoDown(params* gp, GroupSynchro* gs)
{
		if(gs->hl_token == 1 && gs->mode == MODE_IDLE)
		{
			uint8_t barrier = 0 ;
						for(int i = 0 ; i < gs->size ; i++)
						{
							if(gs->vcs[i]->mot->trans.state != STATE_IN)
								barrier = 1 ;
						}
			if(barrier == 1)
				MR_startTargetLeft(gp, gs, 320) ;
			uint8_t tx1[] = {COM_2} ;
			HAL_UART_Transmit(&(gp->huarts[0]), tx1, 1, 1000) ;
			gs->hl_token = 2 ;
		}
		if(gs->hl_token == 2 && gs->MR_mode == MR_MODE_IDLE)
		{
			MR_startReachTop(gs) ;
			gs->hl_token = 3 ;
		}
		if(gs->hl_token == 3 &&  gs->MR_mode == MR_MODE_IDLE)
		{
			if(GS_isOut(&(gp->gss[0])))
				MR_startGoIn(&(gp->gss[0])) ;
			if(GS_isOut(&(gp->gss[2])))
				MR_startGoIn(&(gp->gss[2])) ;
			gs->hl_token = 4 ;
		}
		if(gs->hl_token == 4 && gp->gss[0].MR_mode == MR_MODE_IDLE && gp->gss[2].MR_mode == MR_MODE_IDLE)
		{
			MR_startReachBottom(gs) ;

			gs->hl_token = 5 ;
		}
		if(gs->hl_token == 5 && gs->MR_mode == MR_MODE_IDLE)
		{
			uint8_t tx1[] = {COM_A} ;
			HAL_UART_Transmit(&(gp->huarts[0]), tx1, 1, 1000) ;
			for(int i = 0 ; i < gs->size ; i++)
			{
				gs->vcs[i]->target_currTour = 0 ;
			}

			gs->hl_token = 6 ;
		}
		if(gs->hl_token == 6 && gs->HL_arg == 2)
		{
			uint8_t powers[gs->size] ;
			uint8_t senses[gs->size] ;
					for(int i = 0 ; i < gs->size ; i++)
					{
						powers[i] = REACH_BOTTOM_POWER ;
						senses[i] = SENSE_UP ;
					}
			MR_startBlindMotion(gs, powers, senses, ESCAMOTING_TIME) ;
			gs->HL_arg = 0 ;
			gs->hl_token = 7 ;
		}
		if(gs->hl_token == 7 && gs->MR_mode == MR_MODE_IDLE)
		{
			uint8_t powers[gs->size] ;
			uint8_t senses[gs->size] ;
			for(int i = 0 ; i < gs->size ; i++)
			{
				powers[i] = REACH_BOTTOM_POWER ;
				senses[i] = SENSE_DOWN ;
			}
			MR_startBlindMotion(gs, powers, senses, 2*ESCAMOTING_TIME) ;
			gs->hl_token = 8 ;
		}
		if(gs->hl_token == 8 && gs->MR_mode == MR_MODE_IDLE)
		{
			for(int i = 0 ; i < gs->size ; i++)
			{
				gs->vcs[i]->target_currTour = 0 ;
			}
			uint8_t tx1[] = {COM_C} ;
			HAL_UART_Transmit(&(gp->huarts[0]), tx1, 1, 1000) ;
			gs->hl_token = 0 ;
			gs->HL_mode = HL_MODE_IDLE ;
		}
}

void HL_StartGetFromUp(GroupSynchro* gs)
{
	if(gs->type == GROUP_TYPE_CAPTOR)
		{
			gs->HL_mode = HL_MODE_GET_FROM_UP ;
			gs->hl_token = 1 ;
		}
}

void HL_GetFromUp(params* gp, GroupSynchro* gs)
{
	if(gs->hl_token == 1 && gs->mode == MODE_IDLE)
	{
		MR_startTargetLeft(gp, gs, 344) ;
		gs->hl_token = 3 ;
	}

	if(gs->hl_token == 3 && gs->MR_mode == MR_MODE_IDLE && gs->HL_arg == 1)
	{
		MR_startReachTop(gs) ;
		gs->hl_token = 4 ;
	}
	if(gs->hl_token == 4 && gs->MR_mode == MR_MODE_IDLE)
	{
		uint8_t tx1[] = {COM_B} ;
		HAL_UART_Transmit(&(gp->huarts[2]), tx1, 1, 1000) ;
		gs->hl_token = 5 ;

	}
	if(gs->hl_token == 5 && gs->HL_arg == 3)
	{
		GS_startTargeting(gs,HIGH_AVSPEED, 20) ;
		gs->HL_arg = 0 ;
		gs->hl_token = 6 ;
	}
	if(gs->hl_token == 6 && gs->mode == MODE_IDLE)
	{
		for(int i = 0 ; i < gs->size ; i++)
		{
			gs->vcs[i]->mot->trans.state = STATE_IN ;
		}
		gs->HL_mode = HL_MODE_IDLE ;
		gs->hl_token = 0 ;
	}



}

//___________________________________________________________GO RIGHT________________________________________
void HL_StartGoRight(GroupSynchro* gs)
{
	if(gs->type == GROUP_TYPE_MOTOR || gs->type == GROUP_TYPE_XMOTOR)
		{
			gs->hl_token = 1 ;
			gs->HL_mode = HL_MODE_GO_RIGHT ;
			gs->HL_arg = 0 ;
		}
}
void HL_GoRight(params* gp, GroupSynchro* gs)
{
	if(gs->hl_token == 1)
	{
//		uint8_t barrier = 0 ;
//					for(int i = 0 ; i < gp->gss[0].size ; i++)
//					{
//						if((gp->gss[0].ms[i]->trans.state == STATE_UP)||(gp->gss[0].ms[i]->trans.state == STATE_DOWN))
//							barrier = 1 ;
//					}
//					if(barrier == 1)
//					{
//						//PREPARATION DE MOUVEMENT SI DEMARRE DEPUIS POSITION HAUTE
////						if(gp->gss[4].vcs[0]->target_currTour >= 300)
//							gs->hl_token = 10 ;
//					}
//					else
						gs->hl_token = 2;
	}
	if(gs->hl_token == 2)
	{
		MR_goHori(gs, 0,SENSE_RIGHT, LONG_DELAY, 0) ;
		uint8_t tx1[] = {COM_3} ;
		HAL_UART_Transmit(&(gp->huarts[0]), tx1, 1, 1000) ;
		gs->hl_token = 3 ;
	}
	if(gs->hl_token == 3 && gs->MR_mode == MR_MODE_IDLE)
	{
		gs->hl_token = 0 ;
		gs->HL_mode = HL_MODE_IDLE ;
	}
	if(gs->hl_token == 10)
	{
		MR_startTargetLeft(gp, &(gp->gss[4]), 23) ;
		gs->hl_token = 11 ;
	}
	if(gs->hl_token == 11 && gp->gss[4].MR_mode == MR_MODE_IDLE)
	{
		GS_startTargeting(&(gp->gss[4]), HIGH_AVSPEED, 55) ;
		gs->hl_token = 12 ;

	}
	if(gs->hl_token == 12 && gp->gss[4].mode == MODE_IDLE)
	{
		MR_startGoIn(&(gp->gss[0])) ;
		MR_goHori(gs,0,SENSE_RIGHT, 2*RETRACT_TIME, 0) ;
		gs->hl_token = 13 ;
	}
	if(gs->hl_token == 13 && gp->gss[0].MR_mode == MR_MODE_IDLE)
	{
		MR_startReachBottom(&(gp->gss[4])) ;
		gs->hl_token = 14 ;
	}
	if(gs->hl_token == 14 && gp->gss[4].MR_mode == MODE_IDLE)
	{
		for(int i = 0 ; i < gp->gss[4].size ; i++)
		{
			gp->gss[4].vcs[i]->target_currTour = 0 ;
		}
		uint8_t ids[] = {3} ;
		addGroup(gp, GROUP_TYPE_CAPTOR, 7, ids, 1) ;
		GS_startTargeting(&(gp->gss[7]), LOW_AVSPEED, gp->gss[7].vcs[0]->target_currTour + 2) ;
		gs->hl_token = 15 ;
	}
	if(gs->hl_token == 15 && gp->gss[7].mode == MODE_IDLE)
	{
		eraseGroup(gp, 7) ;
		gs->hl_token = 2 ;
	}
}

void HL_StartGetFromLeft(GroupSynchro* gs)
{
	if(gs->type == GROUP_TYPE_MOTOR || gs->type == GROUP_TYPE_XMOTOR)
		{
			gs->hl_token = 1 ;
			gs->HL_mode = HL_MODE_GET_FROM_LEFT ;
			gs->HL_arg = 0 ;
		}

}

void HL_GetFromLeft(params* gp, GroupSynchro* gs)
{
	if(gs->hl_token == 1)
	{
		MR_goHori(gs, 0, SENSE_RIGHT, LONG_DELAY, 0) ;
		gs->hl_token = 2 ;
	}
	if(gs->hl_token == 2 && gs->MR_mode == MR_MODE_IDLE)
	{
		gs->hl_token = 0 ;
		gs->HL_mode = HL_MODE_IDLE ;
	}
}


//___________________________________________________________GO LEFT________________________________________
void HL_StartGoLeft(GroupSynchro* gs)
{
	if(gs->type == GROUP_TYPE_MOTOR || gs->type == GROUP_TYPE_XMOTOR)
		{
			gs->hl_token = 1 ;
			gs->HL_mode = HL_MODE_GO_LEFT ;
			gs->HL_arg = 0 ;
		}
}
void HL_GoLeft(params* gp, GroupSynchro* gs)
{
	if(gs->hl_token == 1)
		{
//			uint8_t barrier = 0 ;
//			for(int i = 0 ; i < gs->size ; i++)
//			{
//				if(gp->gss[4].vcs[i]->mot->trans.state == STATE_IN)
//					barrier = 1 ;
//			}
//			if(barrier == 1)
//			{
//				//PREPARATION DE MOUVEMENT SI DEMARRE DEPUIS POSITION HAUTE
//				if(gp->gss[4].vcs[0]->target_currTour >= 300)
//					gs->hl_token = 10 ;
//			}
//			else
		uint8_t ids[] = {1} ;
		addGroup(gp, GROUP_TYPE_CAPTOR, 5, ids, 1) ;
		GS_startTargeting(&(gp->gss[5]), LOW_AVSPEED, gp->gss[5].vcs[0]->target_currTour + 2) ;
		gs->hl_token = 2;
		}
	// DEMARRAGE DU MOUVEMENT
		if(gs->hl_token == 2 && gp->gss[5].mode == MODE_IDLE)
		{
			eraseGroup(gp, 5) ;
			MR_goHori(gs,0, SENSE_LEFT, LONG_DELAY, 0) ;
			uint8_t tx1[] = {COM_4} ;
			HAL_UART_Transmit(&(gp->huarts[1]), tx1, 1, 1000) ;
			gs->hl_token = 3 ;
		}
		if(gs->hl_token == 3 && gs->MR_mode == MR_MODE_IDLE)
		{
			gs->hl_token = 0 ;
			gs->HL_mode = HL_MODE_IDLE ;
		}

		//REMISE EN PLACE DEPUIS TAQUET PRIS EN POSITION HAUTE ET TRANSLATEUR MAL SORTIS
		if(gs->hl_token == 10)
		{
			MR_startReachTop(&(gp->gss[4])) ;
			gs->hl_token = 11 ;
		}

		if(gs->hl_token == 11 && gp->gss[4].MR_mode == MR_MODE_IDLE)
		{
			MR_goHori(gs, 0,SENSE_LEFT, RETRACT_TIME*2, 0) ;
			gs->hl_token = 12 ;
		}
		//REMISE EN PLACE DEPUIS TAQUET PRIS EN POSITION HAUTE ET TRANSLATEUR BIEN SORTIS
		if(gs->hl_token == 12 && gs->MR_mode == MR_MODE_IDLE)
		{
			GS_startTargeting(&(gp->gss[4]),HIGH_AVSPEED, TOP_LANDING_LEVEL) ;
			gs->hl_token = 13 ;
		}
		if(gs->hl_token == 13 && gp->gss[4].mode == MODE_IDLE)
		{
			GS_startTargeting(&(gp->gss[4]),LOW_AVSPEED, TOP_PASSAGE_LEVEL) ;
			gs->hl_token = 14 ;
		}
			//---> LIBÉRATION DU PASSAGE PERPENDICULAIRE
		if(gs->hl_token == 14 && gp->gss[4].mode == MODE_IDLE)
		{
			uint8_t ids[] = {1} ;
			addGroup(gp, GROUP_TYPE_CAPTOR, 5, ids, 1) ;
			GS_startTargeting(&(gp->gss[5]), LOW_AVSPEED, TOP_PASSAGE_LEVEL + 2) ;
			gs->hl_token = 15 ;
		}
		if(gs->hl_token == 15 && gp->gss[5].mode == MODE_IDLE)
		{
			for(int i = 0 ; i < gp->gss[4].size ; i++)
			{
				gp->gss[4].vcs[i]->mot->trans.state = STATE_LEFT ;
			}
			for(int i = 0 ; i < gp->gss[5].size ; i++)
			{
				gp->gss[5].vcs[i]->mot->trans.state = STATE_RIGHT;
			}
			eraseGroup(gp, 5) ;
			gs->hl_token = 2 ;
		}
}

void HL_StartGetFromRight(GroupSynchro* gs)
{
	if(gs->type == GROUP_TYPE_MOTOR || gs->type == GROUP_TYPE_XMOTOR)
		{
			gs->hl_token = 1 ;
			gs->HL_mode = HL_MODE_GET_FROM_RIGHT ;
			gs->HL_arg = 0 ;
		}
}

void HL_GetFromRight(params* gp, GroupSynchro* gs)
{
		if(gs->hl_token == 1)
		{
			MR_goHori(gs,0, SENSE_LEFT, LONG_DELAY, 0) ;
			gs->hl_token = 2 ;
		}
		if(gs->hl_token == 2 && gs->MR_mode == MR_MODE_IDLE)
		{
			gs->hl_token = 0 ;
			gs->HL_mode = HL_MODE_IDLE ;
		}
}
//_______________________________________________________GO FORWARD__________________________________________
void HL_StartGoForward(GroupSynchro* gs)
{
	if(gs->type == GROUP_TYPE_MOTOR || gs->type == GROUP_TYPE_XMOTOR)
		{
			gs->hl_token = 1 ;
			gs->HL_mode = HL_MODE_GO_FORWARD ;
			gs->HL_arg = 0 ;
		}
}

void HL_GoForward(params* gp, GroupSynchro* gs)
{
	if(gs->hl_token == 1)
	{
		uint8_t tx1[] = {COM_8} ;
		HAL_UART_Transmit(&(gp->huarts[1]), tx1, 1, 1000) ;
		gs->hl_token = 2 ;
	}
	if(gs->hl_token == 2 && gs->HL_arg == 1)
	{
		gs->HL_arg = 0 ;
		MR_goHori(gs, 10, SENSE_FORWARD, ACCEL_DELAY, 0) ;
		gs->hl_token = 3 ;
	}
	if(gs->hl_token == 3 && gs->MR_mode == MR_MODE_IDLE)
	{
		MR_goHori(gs,0, SENSE_FORWARD, SHORT_DELAY, 0) ;
		gs->hl_token = 4 ;
	}
	if(gs->hl_token == 4 && gs->MR_mode == MR_MODE_IDLE)
	{
		gs->HL_arg = 0 ;
		MR_goHori(gs,10, SENSE_FORWARD, ACCEL_DELAY, 0) ;
		gs->hl_token = 5 ;
	}
	if(gs->hl_token == 5 && gs->MR_mode == MR_MODE_IDLE)
	{
		gs->hl_token = 0 ;
		gs->HL_mode = HL_MODE_IDLE ;
	}
}

void HL_StartGetFromBackward(GroupSynchro* gs)
{
	if(gs->type == GROUP_TYPE_MOTOR || gs->type == GROUP_TYPE_XMOTOR)
		{
			gs->hl_token = 1 ;
			gs->HL_mode = HL_MODE_GET_FROM_BACKWARD ;
			gs->HL_arg = 0 ;
		}
}

void HL_GetFromBackward(params* gp, GroupSynchro* gs)
{
	if(gs->hl_token == 1){
		uint8_t ids1[] = {1} ;
		addGroup(gp, GROUP_TYPE_CAPTOR, 5, ids1, 1) ;
		GS_startTargeting(&(gp->gss[5]), LOW_AVSPEED, gp->gss[5].vcs[0]->target_currTour - 2) ;
		uint8_t ids2[] = {5} ;
		addGroup(gp, GROUP_TYPE_MOTOR, 3, ids2, 1) ;
		MR_startGoIn(&(gp->gss[3])) ;

		gs->hl_token = 2 ;
	}
	if(gs->hl_token == 2 && gp->gss[3].MR_mode == MR_MODE_IDLE && gp->gss[5].mode == MODE_IDLE)
	{
		eraseGroup(gp, 5) ;
		eraseGroup(gp, 3) ;

		uint8_t tx1[] = {COM_A} ; //IS READY
		HAL_UART_Transmit(&(gp->huarts[0]), tx1, 1, 1000) ;

		MR_goHori(gs,10, SENSE_FORWARD, ACCEL_DELAY, 1) ;
		gs->hl_token = 3 ;
	}
	if(gs->hl_token == 3 && gs->MR_mode == MR_MODE_IDLE)
	{
		gs->hl_token = 4 ;
		MR_goHori(gs,0, SENSE_FORWARD, SHORT_DELAY, 1) ;
	}
	if(gs->hl_token == 4 && gs->MR_mode == MR_MODE_IDLE)
	{
		MR_goHori(gs,10, SENSE_FORWARD, ACCEL_DELAY, 1) ;
		gs->hl_token = 5 ;
	}
	if(gs->hl_token == 5 && gs->MR_mode == MR_MODE_IDLE)
	{
		gs->hl_token = 0 ;
		gs->HL_mode = HL_MODE_IDLE ;
	}
}

//_______________________________________________________GO BACKWARD__________________________________________
void HL_StartGoBackward(GroupSynchro* gs)
{
	if(gs->type == GROUP_TYPE_MOTOR || gs->type == GROUP_TYPE_XMOTOR)
		{
			gs->hl_token = 1 ;
			gs->HL_mode = HL_MODE_GO_BACKWARD ;
			gs->HL_arg = 0 ;
		}
}
//
void HL_GoBackward(params* gp, GroupSynchro* gs)
{
	if(gs->hl_token == 1)
	{
		uint8_t tx1[] = {COM_5} ;
		HAL_UART_Transmit(&(gp->huarts[0]), tx1, 1, 1000) ;
		gs->hl_token = 2 ;
	}

	if(gs->hl_token == 2 && gs->MR_mode == MR_MODE_IDLE)
	{
		HAL_Delay(1000) ;
		gs->hl_token = 3 ;
		MR_goHori(gs,5, SENSE_BACKWARD, ACCEL_DELAY, 1) ;
	}
	if(gs->hl_token == 3 && gs->MR_mode == MR_MODE_IDLE)
	{
		gs->hl_token = 4 ;
		MR_goHori(gs,0, SENSE_BACKWARD, SHORT_DELAY, 1) ;
	}
	if(gs->hl_token == 4 && gs->MR_mode == MR_MODE_IDLE)
	{
		MR_goHori(gs,5, SENSE_BACKWARD, ACCEL_DELAY, 1) ;
		gs->hl_token = 5 ;
	}
	if(gs->hl_token == 5 && gs->MR_mode == MR_MODE_IDLE)
	{
		for(int i = 0 ; i < gs->size ; i++)
		{
			gs->ms[i]->trans.state = STATE_DOWN ;
		}
		gs->hl_token = 0 ;
		gs->HL_mode = HL_MODE_IDLE ;
	}
}

void HL_StartGetFromForward(GroupSynchro* gs)
{
	if(gs->type == GROUP_TYPE_MOTOR || gs->type == GROUP_TYPE_XMOTOR)
		{
			gs->hl_token = 1 ;
			gs->HL_mode = HL_MODE_GET_FROM_FORWARD ;
			gs->HL_arg = 0 ;
		}
}

void HL_GetFromForward(params* gp, GroupSynchro* gs)
{
	if(gs->hl_token == 1)
			{
//				uint8_t barrier = 0 ;
//				for(int i = 0 ; i < gs->size ; i++)
//				{
//					if(gp->gss[2].vcs[i]->mot->trans.state == STATE_IN)
//						barrier = 1 ;
//				}
//				if(barrier == 1)
//				{
//					//PREPARATION DE MOUVEMENT SI DEMARRE DEPUIS POSITION HAUTE
//					if(gp->gss[4].vcs[0]->target_currTour >= 300)
						gs->hl_token = 10 ;
//				}
//				else
//					gs->hl_token = 2;

			}
	if(gs->hl_token == 2 && gp->gss[3].MR_mode == MR_MODE_IDLE && gp->gss[6].mode == MODE_IDLE)
		{
			eraseGroup(gp,3) ;
			eraseGroup(gp, 6) ;
			MR_goHori(gs,10, SENSE_BACKWARD, ACCEL_DELAY, 1) ;
			gs->hl_token = 3 ;
		}
	if(gs->hl_token == 3 && gs->MR_mode == MR_MODE_IDLE)
		{
			gs->hl_token = 4 ;
			MR_goHori(gs,0, SENSE_BACKWARD, SHORT_DELAY, 1) ;
		}
		if(gs->hl_token == 4 && gs->MR_mode == MR_MODE_IDLE)
		{
			MR_goHori(gs,10, SENSE_BACKWARD, ACCEL_DELAY, 1) ;
			gs->hl_token = 5 ;
		}
		if(gs->hl_token == 5 && gs->MR_mode == MR_MODE_IDLE)
		{
			for(int i = 0 ; i < gs->size ; i++)
			{
				gs->ms[i]->trans.state = STATE_DOWN ;
			}
			gs->hl_token = 0 ;
			gs->HL_mode = HL_MODE_IDLE ;
		}
		if(gs->hl_token == 10)
		{
			uint8_t ids1[] = {7} ;
			addGroup(gp, GROUP_TYPE_MOTOR, 3, ids1, 1) ;
			MR_startGoIn(&(gp->gss[3])) ;
			uint8_t ids2[] = {3} ;
			addGroup(gp, GROUP_TYPE_CAPTOR, 6, ids2, 2) ;
			GS_startTargeting(&(gp->gss[6]), LOW_AVSPEED, gp->gss[6].vcs[0]->target_currTour -2) ;
			gs->hl_token = 2 ;
		}
}

//_______________________________________________________________GO BACK STOP___________________________________

void HL_startGoBackStop(params* gp, GroupSynchro* gs, uint8_t sense)
{
	uint8_t senses[gs->size] ;
	for(int i = 0 ; i < gs->size ; i++)
	{
		senses[i] = sense ;
	}
	MR_startBackStopMotion(gs, gs->powers, senses) ;
	gs->HL_mode = HL_MODE_BS_MOTION ;
	gs->hl_token = 1 ;
}

void HL_goBackStop(GroupSynchro* gs)
{
	if(gs->MR_mode == MR_MODE_IDLE)
	{
		gs->hl_token = 0 ;
		gs->HL_mode = HL_MODE_IDLE ;
	}
}

void HL_handle_message(params* gp, uint8_t* buf)
{
	for(int i = 0 ; i < NUMGROUPS ; i++)
	{
		if(gp->gss[i].HL_mode == HL_MODE_GO_UP)
		{
			if(buf[0] == COM_A)
			{
				gp->gss[i].HL_arg = 1 ;
			}
		}
		if(gp->gss[i].HL_mode == HL_MODE_GET_FROM_DOWN)
		{
			if(buf[0] == COM_A)
			{
				gp->gss[i].HL_arg = 2 ; ;
			}

			if(buf[0] == COM_C)
			{
				gp->gss[i].HL_arg = 3 ; ;
			}
		}
		if(gp->gss[i].HL_mode == HL_MODE_GO_DOWN)
		{
			if(buf[0] == COM_B)
			{
				gp->gss[i].HL_arg = 2 ;
			}
		}
		if(gp->gss[i].HL_mode == HL_MODE_GET_FROM_UP)
		{
			if(buf[0] == COM_A)
			{
				gp->gss[i].HL_arg = 1 ; ;
			}

			if(buf[0] == COM_C)
			{
				gp->gss[i].HL_arg = 3 ; ;
			}
		}
		if(gp->gss[i].HL_mode == HL_MODE_GO_FORWARD)
		{
			if(buf[0] == COM_A)
			{
				gp->gss[i].HL_arg = 1 ;
			}
		}
	}
}
