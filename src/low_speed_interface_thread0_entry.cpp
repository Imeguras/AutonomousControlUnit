
#include <string.h>
#include <functional>
#include <stdlib.h>
#include <low_speed_interface_thread0.h>

#include "HighSpeedAbsL.cpp"
#include "CanRen.h"
#include "utils.h"
can_frame_t frame;
can_info_t info;
static volatile bool _is_sent = false;
static volatile bool _is_error =false;
/* CANFD Channel 1 Acceptance Filter List (AFL) rule array */
extern "C" const canfd_afl_entry_t p_canfd0_afl[CANFD_CFG_AFL_CH1_RULE_NUM] ={
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

extern "C" void canfd_callback(can_callback_args_t * p_args);
void low_speed_interface_thread0_entry(void) {

     volatile UINT status = R_CANFD_Open(&g_canfd0_ctrl, &g_canfd0_cfg);

     status = R_CANFD_InfoGet(&g_canfd0_ctrl, &info);
     if(info.error_code){
    	 led_update(red, BSP_IO_LEVEL_HIGH);
     }


    frame.id = 0x40;
    frame.id_mode = CAN_ID_MODE_STANDARD;
    frame.type = CAN_FRAME_TYPE_DATA;
    frame.data_length_code = 8U;
    frame.options = 0;

	/* Update transmit frame data with message */
    frame.data[0]=0x41;
    frame.data[1]=0x41;
    frame.data[2]=0x41;
    frame.data[3]=0x41;
    frame.data[4]=0x41;
    frame.data[5]=0x41;
    frame.data[6]=0x41;
    frame.data[7]=0x41;
	 status = R_CANFD_Write(&g_canfd0_ctrl, 0, &frame);
	 while(!_is_sent){

	 }
	 led_update(green, BSP_IO_LEVEL_HIGH);
}

/* Callback function */
void canfd0_callback(can_callback_args_t *p_args){
	switch (p_args->event){
	case CAN_EVENT_TX_COMPLETE:
		_is_sent=true;
		break;
	case CAN_EVENT_ERR_WARNING:          /* error warning event */
	case CAN_EVENT_ERR_PASSIVE:          /* error passive event */
	case CAN_EVENT_ERR_BUS_OFF:          /* error bus off event */
	case CAN_EVENT_BUS_RECOVERY:         /* Bus recovery error event */
	case CAN_EVENT_MAILBOX_MESSAGE_LOST: /* overwrite/overrun error event */
	case CAN_EVENT_ERR_BUS_LOCK:         /* Bus lock detected (32 consecutive dominant bits). */
	case CAN_EVENT_ERR_CHANNEL:          /* Channel error has occurred. */
	case CAN_EVENT_TX_ABORTED:           /* Transmit abort event. */
	case CAN_EVENT_ERR_GLOBAL:           /* Global error has occurred. */
	case CAN_EVENT_TX_FIFO_EMPTY:        /* Transmit FIFO is empty. */
	{
	  _is_error = true;
	  break;
	}

	}
}

