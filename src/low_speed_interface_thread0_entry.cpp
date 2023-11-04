#include <low_speed_interface_thread0.h>
#include "HighSpeedAbsL.cpp"
#include "CanRen.h"
#include <string.h>
#include <functional>
static can_frame_t temp;
/* Micro Ros entry function */
void low_speed_interface_thread0_entry(void) {
	/*HighSpeed_AbsL<CanRen> can;
	st_can_frame blah;
	blah.id = 0x41;
	uint64_t data= 0x1100220033004400;

	blah.type = CAN_FRAME_TYPE_DATA;
	blah.data_length_code = 8;
	blah.id_mode =CAN_ID_MODE_STANDARD;
	memcpy(blah.data,(void *) &data, sizeof(data));
	volatile UINT k=0;
	k = can->write((void *)&blah, blah.data_length_code);
	*/
	while (1) {


		tx_thread_sleep(1);
	}
}

/* Callback function */
void can_callback(can_callback_args_t *p_args){
	/**if (can_ptr != nullptr){
		(*can_ptr)->can_callback(p_args);
	}**/
    switch (p_args->event) {


        case CAN_EVENT_TX_COMPLETE:


            break;


        case CAN_EVENT_RX_COMPLETE:
            //b_can_rx = true;
        	//rx_ready=true;


            memmove(&temp, &p_args->frame, sizeof(can_frame_t));  //copy the received data to rx_frame
           // this->buffered_mails_rx.push_back(temp);
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

            //b_can_err = true;                   //set flag bit
            break;
    }

}
