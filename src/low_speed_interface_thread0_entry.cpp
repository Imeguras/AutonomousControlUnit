
#include <string.h>
#include <stdlib.h>
#include <low_speed_interface_thread0.h>

#include "Interfaces/HighSpeedAbsL.cpp"
#include "Interfaces/Drivers/HardwareBased/CanFDRen.h"
#include "utils.h"
#include "Data_structs/AutomataStructs.hpp"
#include "Data_structs/Can-Header-Map/CAN_asdb.h"
#include "Data_structs/Store.h"



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
              .rx_buffer   = (canfd_rx_mb_t) CANFD_RX_BUFFER_FIFO_0,
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
              .rx_buffer   = (canfd_rx_mb_t) CANFD_RX_BUFFER_FIFO_1,
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
                   .rx_buffer   = (canfd_rx_mb_t) CANFD_RX_BUFFER_FIFO_0,
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
                   .rx_buffer   = (canfd_rx_mb_t) CANFD_RX_BUFFER_FIFO_1,
                   .fifo_select_flags = CANFD_RX_FIFO_1,
                 }
             }

};

extern "C" void canfd0_callback(can_callback_args_t * p_args);
extern "C" void canfd1_callback(can_callback_args_t * p_args);

void low_speed_interface_thread0_entry(void) {

    HighSpeed_AbsL<CanFDRen> canfd0;
    HighSpeed_AbsL<CanFDRen> canfd1;

    interface_callback0_t=(void *)&canfd0;

    interface_callback1_t=(void *)&canfd1;


    can_frame_t frame;
    frame.id = NMT_ADDRESS_COBID();
    frame.data_length_code = 8U;

    frame.id_mode  = CAN_ID_MODE_STANDARD;
    frame.type = CAN_FRAME_TYPE_DATA;

    frame.options = 0;

    can_frame_stream _data = canfd1->currentCanOpenStack->nmt_message(NMT_ENTER_PRE_OPERATIONAL, 0);
    memcpy(frame.data, &_data.data, 8);
    canfd1->write((void *)&frame,0);
    R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
    _data = canfd1->currentCanOpenStack->nmt_message(NMT_START_REMOTE_NODE, 0);
    memcpy(frame.data, &_data.data, 8);
    canfd1->write((void *)&frame,0);
    R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);

    frame.id = SDO_REQUEST_ADDRESS_COBID();
    can_frame_stream _data2 = canfd1->currentCanOpenStack->requestStatusWordMessage();
    memcpy(frame.data, &_data2.data, 8);
    canfd1->write((void *)&frame,0);




    //initialize the frame
    while (canfd1->currentCanOpenStack->g_currentState () != Switch_on_disabled ){
        R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
        //canfd1->decodeImmediate (frame);
    }





    //frame.options = CANFD_FRAME_OPTION_BRS | CANFD_FRAME_OPTION_FD;
    /*critical_as temp_data;
     *
      tx_semaphore_get(&css, 32);
          temp_data = store::critical_autonomous_system_status;
      tx_semaphore_put(&css);*/





    R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
    while(1){

        led_blink(1, 1);
        canfd1->write((void *)&frame,0);

        R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MILLISECONDS);

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

