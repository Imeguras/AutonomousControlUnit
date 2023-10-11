/*
 * CanRen.cpp
 *
 *  Created on: 09/10/2023
 *      Author: micron
 */

#include "CanRen.h"


CanRen::CanRen()  {

}

CanRen::~CanRen() {
	//UINT status = R_CAN_Close((can_instance_ctrl_t *) &g_can0_ctrl);
	//this->error_counter+=status;
}

int CanRen::initialization(){

	UINT status = R_CAN_Open((can_instance_ctrl_t *)&g_can0_ctrl, (can_cfg_t *)&g_can0_cfg);
	//can_callback_args_t callback;
	//status = R_CAN_CallbackSet((void *)&g_can0_ctrl, (void *)&callback, p_context, p_callback_memory);
	this->error_counter+=status;
	this->mailbox_address = 0x0000;
	return this->error_counter;

	return 0;
}

void* CanRen::recv(void * data, uint32_t stream_size){
	/*TODO*/
	(void) data;
	(void) stream_size;
	return NULL;

}
uint32_t CanRen::write(void *data, uint32_t stream_size){
	uint32_t ret;
	(void) stream_size;
	//TODO something that is buffered instead of unbuffered through mailbox
	ret = R_CAN_Write((can_instance_ctrl_t *) &g_can0_ctrl, 0, (st_can_frame *)data );

	return ret;

}


/* Callback function */
void CanRen::can_callback(can_callback_args_t *p_args){

}/* Callback function */


