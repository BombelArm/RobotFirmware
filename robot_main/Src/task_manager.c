/*! \file
 * \brief Task manager source code.
 */


#include "task_manager.h"

void t_taskManagerInit(){
	task newtask;

	HAL_GPIO_WritePin(MOTORS_ENABLE_GPIO_Port,MOTORS_ENABLE_Pin, !MOTORS_ENABLED);
	HAL_GPIO_WritePin(FANS_ENABLE_GPIO_Port,FANS_ENABLE_Pin, !FANS_ENABLED);

}

void t_append_task(uint8_t msg[ORDER_LENGTH]){

	uint8_t hw_task_type,order_type;
	uint8_t dir_flag;
	int16_t f0,f1,f2;
	task newtask;

	if(buffer_act_size == TASK_BUFFER_SIZE){
		c_sendCallback(RESP_ORDER_BUFFER_OVERFLOW);
		return;
	}

	order_type=(int)(msg[0] & 0x7 );
	newtask.order_type=order_type;


	switch(order_type){
			case MSG_HW_CONFIG:
				newtask.hw_task_type=msg[1];
				break;
			case MSG_JOINT_SPACE:

				dir_flag=msg[0]>>3;
				f0=(msg[1]<<4)+(msg[2]>>4);
				f1=((msg[2] & 0x00F)<<8) + (msg[3]);
				f2=(msg[4]<<4) + msg[5];

				if(dir_flag & 1){
					f0=-f0;
				}
				if(dir_flag>>1 & 1){
					f1=-f1;
				}
				if(dir_flag>>2 & 1){
					f2=-f2;
				}

				if(m_validatePosition(0,msg[6],f0) != 0){
					c_sendCallback(RESP_ORDER_ERROR);
					return;
				}else if(m_validatePosition(1,msg[7],f2) != 0){
					c_sendCallback(RESP_ORDER_ERROR);
					return;
				}else if(m_validatePosition(2,msg[8],f2) != 0){
					c_sendCallback(RESP_ORDER_ERROR);
					return;
				}

				newtask.f0=f0;
				newtask.f1=f1;
				newtask.f2=f2;

				newtask.speed0=msg[6];
				newtask.speed1=msg[7];
				newtask.speed2=msg[8];

				break;
			case MSG_OPERATION_SPACE:
				break;
	}

	buffer[buffer_act_size++]=newtask;
	t_exec();
}


void t_exec(){
	if(buffer_act_size == 0) return;

	task task1=buffer[0];

	switch(task1.order_type){
			case MSG_HW_CONFIG:
				switch(task1.hw_task_type){
					case HW_MOTORS_ON:
						HAL_GPIO_WritePin(MOTORS_ENABLE_GPIO_Port,MOTORS_ENABLE_Pin, MOTORS_ENABLED);
						break;
					case HW_MOTORS_OFF:
						HAL_GPIO_WritePin(MOTORS_ENABLE_GPIO_Port,MOTORS_ENABLE_Pin, !MOTORS_ENABLED);
						break;
					case HW_FANS_ON:
						HAL_GPIO_WritePin(FANS_ENABLE_GPIO_Port,FANS_ENABLE_Pin, FANS_ENABLED);
						break;
					case HW_FANS_OFF:
						HAL_GPIO_WritePin(FANS_ENABLE_GPIO_Port,FANS_ENABLE_Pin, !FANS_ENABLED);
						break;
				}
				t_shift_buffer(); // removing task that is done
				break;

			case MSG_JOINT_SPACE:

				if (motion_nodes[0].position_reached!=1 || motion_nodes[1].position_reached!=1 || motion_nodes[2].position_reached!=1){
					return;
				}

				m_setPosition(0,task1.f0,task1.speed0);
				m_setPosition(1,task1.f1,task1.speed1);
				m_setPosition(2,task1.f2,task1.speed2);
				t_shift_buffer();

				break;
			case MSG_OPERATION_SPACE:
				break;
	}
}

void t_shift_buffer(){
	for(int i=0;i<TASK_BUFFER_SIZE-1;i++){
		buffer[i]=buffer[i+1];
	}
	buffer_act_size--;
}
