/*
 * motor.h
 *
 *  Created on: 15 janv. 2020
 *      Author: Samuel
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#define NUMPOWERS 3

#define MOTPERIOD 500

#include "stm32g0xx_hal.h"
#include "translateur.h"

typedef struct Motor
{
	uint8_t id ;
	uint16_t cpower ;
	uint16_t tpower ;
	uint16_t sPowers[NUMPOWERS] ;

	uint8_t sense ;
	uint8_t tsense ;
	GPIO_TypeDef* pinFam1;
	uint16_t pinNum1 ;
	GPIO_TypeDef* pinFam2 ;
	uint16_t pinNum2 ;
	TIM_HandleTypeDef htim ;
	uint32_t channel ;
	GPIO_TypeDef* pinFamTim ;
	uint16_t pinNumTim ;
	uint8_t runTim ;
	uint32_t last_tick ;
	uint32_t speedUpTime ;
	uint32_t last_update ;
	uint8_t state ;
	Translateur trans ;
}Motor;

void updateMotor(Motor* mot, uint32_t time_us) ;
void setMotorPower(Motor* mot, uint16_t power, uint32_t time_us) ;
void setMotorTPower(Motor* mot, uint16_t power) ;
void setMotorTSense(Motor* mot, uint8_t sense) ;
void standardInitMotor(Motor* m) ;



#endif /* MOTOR_H_ */
