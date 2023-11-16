#include <low_speed_interface_thread0.h>
#include "HighSpeedAbsL.cpp"
#include "CanRen.h"
#include <string.h>
#include <functional>
#include <stdlib.h>

//#include "SEGGER_RTT/SEGGER_RTT.h";
static can_frame_t temp;
//#define APP_PRINT(fn_,...) (SEGGER_RTT_printf(SEGGER_INDEX,(fn_),##__VA_ARGS__))
static volatile bool rx=false;
/* Micro Ros entry function */
void low_speed_interface_thread0_entry(void) {
	/*HighSpeed_AbsL<CanRen> can;
	can_frame_t blah;
	//blah = temp;
						blah.id = 0x43;
						uint64_t data= 0x1100220033004400;

						blah.type = CAN_FRAME_TYPE_DATA;
						blah.data_length_code = 8;
						blah.id_mode =CAN_ID_MODE_STANDARD;
						auto k = 0xAABBCCDDEEFF1122;
						memcpy(blah.data,(void *)k ,8);


						volatile UINT f=0;
						f = can->write((void *)&blah, blah.data_length_code);
	while(1){
		if(rx){
			rx = false;
			blah.id = 0x43;
			uint64_t data= 0x1100220033004400;

			blah.type = CAN_FRAME_TYPE_DATA;
			blah.data_length_code = 8;
			blah.id_mode =CAN_ID_MODE_STANDARD;
			memcpy(blah.data,(void *) &data, sizeof(data));
			blah.data[0]=temp.data[0];
			blah.data[1]=temp.data[1];
			blah.data[2]=temp.data[2];
			blah.data[3]=temp.data[3];
			blah.data[4]=temp.data[4];
			f=0;
			f = can->write((void *)&blah, blah.data_length_code);

			//APP_PRINT("FUNFOU");
		}
		tx_thread_sleep(1);
	}*/
	/*	*/
	/*while (1) {
		tx_thread_sleep(1);
//	}*/
}

/* Callback function */
void can_callback(can_callback_args_t *p_args){
	/**if (can_ptr != nullptr){
		(*can_ptr)->can_callback(p_args);
	}**/
	switch (p_args->event) {
		default:
			auto k = p_args->event;
			printf("hello");
			break;
	}
/*    switch (p_args->event) {


        case CAN_EVENT_TX_COMPLETE:


            break;


        case CAN_EVENT_RX_COMPLETE:
        	rx=true;
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
    }*/

}
