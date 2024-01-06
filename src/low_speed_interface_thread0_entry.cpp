
#include <string.h>
#include <functional>
#include <stdlib.h>
#include <low_speed_interface_thread0.h>

#include "HighSpeedAbsL.cpp"
#include "interfaces/CanFDRen.h"
#include "utils.h"
#include "data_structs/AutomataStructs.hpp"
#include "data_structs/chm/CAN_asdb.h"
#include "data_structs/Store.h"
volatile can_frame_t frame;
can_info_t info;
static volatile bool _is_sent = false;
static volatile bool _is_error =false;

void * interface_callback_t;
/*CANFD Channel 1 Acceptance Filter List (AFL) rule array */
extern "C" const canfd_afl_entry_t p_canfd0_afl[CANFD_CFG_AFL_CH0_RULE_NUM] ={
		{
		    .id =
		      {
		        .id = 0x1FFFFFFF,
		        .frame_type = CAN_FRAME_TYPE_DATA,
		        .id_mode    = CAN_ID_MODE_EXTENDED,
		      },
		    .mask =
		      {
		        .mask_id         = 0,
		        .mask_frame_type = 1,
		        .mask_id_mode    = 1,
		      },
		    .destination =
		      {
		        .minimum_dlc = CANFD_MINIMUM_DLC_0,
		        .rx_buffer   = (canfd_rx_mb_t) CANFD_RX_MB_0,
		        .fifo_select_flags = CANFD_RX_FIFO_0,
		      }
		},{
		    .id =
		      {
		        .id = 0x1FFFFFFF,
		        .frame_type = CAN_FRAME_TYPE_DATA,
		        .id_mode    = CAN_ID_MODE_STANDARD,
		      },
		    .mask =
		      {
		        .mask_id         = 0,
		        .mask_frame_type = 1,
		        .mask_id_mode    = 1,
		      },
		    .destination =
		      {
		        .minimum_dlc = CANFD_MINIMUM_DLC_0,
		        .rx_buffer   = (canfd_rx_mb_t) CANFD_RX_MB_1,
		        .fifo_select_flags = CANFD_RX_FIFO_1,
		      }
		  }

};

//extern "C" void canfd_callback(can_callback_args_t * p_args);
void low_speed_interface_thread0_entry(void) {

	HighSpeed_AbsL<CanFDRen> canfd;
	//canfd->initialization();
	interface_callback_t=(void *)&canfd;
	//void (*canfdCBHandle)(can_callback_args_t *)= callbackWrapper(can_callback_args_t *,canfd);
    frame.id = CAN_AS_STATUS;
    frame.id_mode = CAN_ID_MODE_STANDARD;
    frame.type = CAN_FRAME_TYPE_DATA;
    frame.data_length_code = 8U;
    frame.options = 0;
    wrapper_int8<critical_as_state> temp_data;
        tx_semaphore_get(&css, 32);
    		temp_data = store::critical_autonomous_system_status;
		tx_semaphore_put(&css);
	MAP_ENCODE_AS_STATE(frame.data,temp_data.state);

/* Update transmit frame data with message */
    canfd->write((void *)&frame,1);
}

/* Callback function */
extern "C" void canfd0_callback(can_callback_args_t *p_args){
	if(interface_callback_t != NULL){
		((CanFDRen *)interface_callback_t)->callbackHandle(p_args);
	}
}

