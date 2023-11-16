/*
 * CanRen.cpp
 *
 *  Created on: 09/10/2023
 *      Author: micron
 */
//#ifdef BOARD_RA6M5_EK
#include "CanRen.h"


/*
CanFDRen::CanFDRen()  {

}

CanFDRen::~CanFDRen() {
	UINT status = R_CANFD_Close((can_instance_ctrl_t *) &g_can0_ctrl);
	this->error_counter+=status;
}

int CanFDRen::initialization(){
	rx_ready=false;
	tx_ready=false;

	UINT status = R_CANFD_Open((can_instance_ctrl_t *)&g_can0_ctrl, (can_cfg_t *)&g_can0_cfg);
		//can_callback_args_t callback;
		//status = R_CAN_CallbackSet((void *)&g_can0_ctrl, (void *)&callback, p_context, p_callback_memory);
	this->error_counter+=status;
	tx_ready=true;
	//this->error_counter += R_CAN_CallbackSet(&g_can0_ctrl, &can_callback, g_can0_cfg.p_context, g_can0_ctrl.p_callback_memory);

	return this->error_counter;
}
/**
 *
 * @param data @b MUST be already allocated...
 * @param stream_size @b HERE stream_size means how many messages are going to be sent over data default is @b one
 */
/*
void* CanFDRen::recv(void * data, uint32_t stream_size=1){
	try{
		while(rx_ready!=true){
			//R_BSP_SoftwareDelay(CANREN_LOOPBACK_TIMEOUT, BSP_DELAY_UNITS_MILLISECONDS);
		}
		size_t i;
		can_frame_t __data_deserialized [stream_size];

		for(i=0; i < stream_size; i++){
			if(this->buffered_mails_rx.empty()){
				rx_ready = false;
				return (void *)data;
			}
			__data_deserialized[i] =this->buffered_mails_rx.front();
			//TODO: some kind of logging SD card?
			//remove the front
			this->buffered_mails_rx.pop_front();
		}
		memmove((can_frame_t *)data,__data_deserialized, sizeof(__data_deserialized));

	}catch(...){
		this->error_counter+=1;
		//TODO: logging!
	}
	return (void *)data;

}
uint32_t CanFDRen::write(void *data, uint32_t stream_size){
	uint32_t ret;
	(void) stream_size;
	//get and remove mailbox
	while(tx_ready!=true){
		R_BSP_SoftwareDelay(CANREN_LOOPBACK_TIMEOUT, BSP_DELAY_UNITS_MILLISECONDS);

	}

	ret = R_CANFD_Write((canfd_instance_ctrl_t *) &g_can0_ctrl, mailbox, (st_can_frame *)data );
	return ret;
}



/* Callback function */
/*
void CanFDRen::canfd_callback(can_callback_args_t *p_args){

    switch (p_args->event) {

    	//TODO CHECK IF CAN FD SUIPPORTS THIS
        case CAN_EVENT_TX_COMPLETE:

        	tx_ready=true;


            break;


        case CAN_EVENT_RX_COMPLETE:
            //b_can_rx = true;
        	rx_ready=true;


        		can_frame_t temp;
            	memmove(&temp, &p_args->frame, sizeof(can_frame_t));  //copy the received data to rx_frame
				break;

        case CAN_EVENT_MAILBOX_MESSAGE_LOST:    //overwrite/overrun error event
        case CAN_EVENT_BUS_RECOVERY:            //Bus recovery error event
        case CAN_EVENT_ERR_BUS_OFF:             //error Bus Off event
        case CAN_EVENT_ERR_PASSIVE:             //error passive event
        case CAN_EVENT_ERR_WARNING:             //error warning event
        case CAN_EVENT_ERR_BUS_LOCK:            //error bus lock
        case CAN_EVENT_ERR_CHANNEL:             //error channel
        case CAN_EVENT_ERR_GLOBAL:              //error global
        case CAN_EVENT_TX_ABORTED:              //error transmit abort
        case CAN_EVENT_TX_FIFO_EMPTY:           //error transmit FIFO is empty
        case CAN_EVENT_FIFO_MESSAGE_LOST:       //error FIFO message lost
        	//TODO actually do something
        	this->error_counter+=1;
            //b_can_err = true;                   //set flag bit
            break;
    }


}*//* Callback function */
//#endif


