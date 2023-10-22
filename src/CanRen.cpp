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

	 auto p_extended = (can_extended_cfg_t *)g_can0_cfg.p_extend;
	 /**
	 * Get available tx mailbox
	 **/
	for (uint32_t i = 0U; i < p_extended->mailbox_count; i++){
	        if (CAN_MAILBOX_RECEIVE ==p_extended->p_mailbox[i].mailbox_type){
	        	this->available_can_mailboxes_rx.push_back(i);
	        }
	        if(CAN_MAILBOX_TRANSMIT ==p_extended->p_mailbox[i].mailbox_type){
	        	this->available_can_mailboxes_tx.push_back(i);
	        }
	}
	return this->error_counter;
}
/**
 *
 * @param data @b MUST be already allocated...
 * @param stream_size @b HERE stream_size means how many messages are going to be sent over data default is @b one
 */
void* CanRen::recv(void * data, uint32_t stream_size=1){
	try{
		size_t i;
		can_frame_t __data_deserialized [stream_size];
		for(i=0; i < stream_size; i++){
			__data_deserialized[i] =this->buffered_mails_rx.front();
			//TODO: some kind of logging SD card?
			//remove the front
			this->buffered_mails_rx.pop_front();
		}
		memmove(data,__data_deserialized, sizeof(__data_deserialized));

	}catch(...){
		//TODO: logging!
	}
	return data;

}
uint32_t CanRen::write(void *data, uint32_t stream_size){
	uint32_t ret;
	(void) stream_size;
	//get and remove mailbox
	int mailbox = this->available_can_mailboxes_tx.front();
	this->available_can_mailboxes_tx.pop_front();

	ret = R_CAN_Write((can_instance_ctrl_t *) &g_can0_ctrl, mailbox, (st_can_frame *)data );
	return ret;

}



/* Callback function */
void CanRen::can_callback(can_callback_args_t *p_args){
    switch (p_args->event) {


        case CAN_EVENT_TX_COMPLETE:
        	this->available_can_mailboxes_tx.push_back(p_args->mailbox);
        	//TODO check if the VCU has received the message
            //b_can_tx = true;        //set flag bit
            break;


        case CAN_EVENT_RX_COMPLETE:
            //b_can_rx = true;
        	can_frame_t temp;
            memcpy(&temp, &p_args->frame, sizeof(can_frame_t));  //copy the received data to rx_frame
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


}/* Callback function */


