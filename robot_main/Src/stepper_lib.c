/*
 * stepper_lib.c
 *
 *  Created on: 09.03.2018
 *      Author: mromanow
 */

#ifndef STEPPER_LIB_C_
#define STEPPER_LIB_C_

#include "stepper_lib.h"

void s_motorsInit(){
	uint16_t pins[STEPPER_N][2]={
			{MOTOR0_STEP_Pin, MOTOR0_DIR_Pin},
			{MOTOR1_STEP_Pin, MOTOR1_DIR_Pin},
			{MOTOR2_STEP_Pin, MOTOR2_DIR_Pin}
	};
	GPIO_TypeDef  *ports[STEPPER_N][2]={
			{MOTOR0_STEP_GPIO_Port, MOTOR0_DIR_GPIO_Port},
			{MOTOR1_STEP_GPIO_Port, MOTOR1_DIR_GPIO_Port},
			{MOTOR2_STEP_GPIO_Port, MOTOR2_DIR_GPIO_Port}
	};

	uint8_t	directions[STEPPER_N]={
			1,
			1,
			1
	};

	uint8_t steps[STEPPER_N]={
			40,
			60,
			4
	};

	for(int i=0;i<STEPPER_N;i++){
		motors[i].step_pin=pins[i][0];
		motors[i].dir_pin=pins[i][1];

		motors[i].step_port=(GPIO_TypeDef *)ports[i][0];
		motors[i].dir_port=(GPIO_TypeDef *)ports[i][1];

		motors[i].max_speed=HW_MOTOR_MAX_SPEED;
		motors[i].min_speed=HW_MOTOR_MIN_SPEED;
		motors[i].clockwise_direction=directions[i];

		motors[i].enabled = 0;
		motors[i].step = steps[i];
	}
}
void s_step(uint8_t motor){

	if(motors[motor].timer_period==0 || motors[motor].enabled != 1)return;
	//if(steps >= 2*200)return;

	motors[motor].timer_counter+=1;

	if(motors[motor].timer_counter == motors[motor].timer_period){
		HAL_GPIO_TogglePin((GPIO_TypeDef *) motors[motor].step_port, motors[motor].step_pin);
		steps++;
		motors[motor].timer_counter=0;
	}

}

void s_stepAll(){

	for(int i=0;i<STEPPER_N;i++){

		if(motors[i].timer_period==0 || motors[i].enabled != 1)continue;
		motors[i].timer_counter+=1;

		if(motors[i].timer_counter == motors[i].timer_period){
			HAL_GPIO_TogglePin((GPIO_TypeDef *) motors[i].step_port, motors[i].step_pin);
			motors[i].timer_counter=0;
		}
	}

}


//speed = 100 -> 1 revolution per second
void s_setSpeed(uint8_t motor,uint32_t speed){
	stepper stepper1=motors[motor];
	int speed1;

	if(speed>stepper1.max_speed){
		speed1=100000/(2*stepper1.step*stepper1.max_speed);
	}else if(speed<stepper1.min_speed){
		speed1=100000/(2*stepper1.step*stepper1.min_speed);
	}else{
		speed1=100000/(2*stepper1.step*speed);
	}

	motors[motor].timer_period=speed1;
}

void s_changeDir(uint8_t motor,uint8_t dir){
	if(dir==1){
		HAL_GPIO_WritePin(motors[motor].dir_port, motors[motor].dir_pin, motors[motor].clockwise_direction);
	}else if(dir==0){
		HAL_GPIO_WritePin(motors[motor].dir_port, motors[motor].dir_pin, !motors[motor].clockwise_direction);
	}
}

void s_enable(uint8_t motor){
	motors[motor].enabled=1;
}

void s_enableAll(){
	for(int i=0;i<STEPPER_N;i++){
			motors[i].enabled=1;
	}
}

void s_disable(uint8_t motor){
	motors[motor].enabled=0;
}

void s_disableAll(){
	for(int i=0;i<STEPPER_N;i++){
				motors[i].enabled=0;
	}
}

#endif /* STEPPER_LIB_C_ */

