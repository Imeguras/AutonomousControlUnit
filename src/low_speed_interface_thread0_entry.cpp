
#include <string.h>
#include <stdlib.h>
#include <low_speed_interface_thread0.h>

#include "Interfaces/HighSpeedAbsL.cpp"
#include "Interfaces/Drivers/HardwareBased/CanFDRen.h"
#include "utils.h"
#include "Data_structs/AutomataStructs.hpp"
#include "Data_structs/Can-Header-Map/CAN_asdb.h"
#include "Data_structs/Store.h"
volatile can_frame_t frame;
can_info_t info;

void * interface_callback0_t;
void * interface_callback1_t;
/* CANFD Channel 1 Acceptance Filter List (AFL) rule array */
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
extern "C" const canfd_afl_entry_t p_canfd1_afl[CANFD_CFG_AFL_CH1_RULE_NUM] ={
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

extern "C" void canfd0_callback(can_callback_args_t * p_args);
void low_speed_interface_thread0_entry(void) {
    HighSpeed_AbsL<CanFDRen> canfd0;
    HighSpeed_AbsL<CanFDRen> canfd1;
    //canfd1->channelInjection((canfd_instance_ctrl_t&)g_canfd1_ctrl, (can_cfg_t&)g_canfd1_cfg);
    interface_callback0_t=(void *)&canfd0;
    interface_callback1_t=(void *)&canfd1;


    frame.id = CAN_AS_STATUS;
    frame.id_mode = CAN_ID_MODE_STANDARD;
    frame.type = CAN_FRAME_TYPE_DATA;
    frame.data[0]= 0x41;
    frame.data[1]= 0x41;
    frame.data[2]= 0x41;
    frame.data[3]= 0x41;
    frame.data[4]= 0x41;
    frame.data_length_code = 8U;
    frame.options = 0;
    /*critical_as temp_data;
     *
      tx_semaphore_get(&css, 32);
          temp_data = store::critical_autonomous_system_status;
      tx_semaphore_put(&css);*/




    canfd0->write((void *)&frame,0);
    MAP_ENCODE_AS_STATE(frame.data,0xFF);
    canfd1->write((void *)&frame,0);
    R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
    while(1){
        R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
    }


}

/* Callback function */
extern "C" void canfd0_callback(can_callback_args_t *p_args){
  if(interface_callback0_t != NULL){
      ((CanFDRen *)interface_callback0_t)->callbackHandle(p_args);
  }
}

extern "C" void canfd1_callback(can_callback_args_t *p_args){
  if(interface_callback1_t != NULL){
      ((CanFDRen *)interface_callback1_t)->callbackHandle(p_args);
  }
}
