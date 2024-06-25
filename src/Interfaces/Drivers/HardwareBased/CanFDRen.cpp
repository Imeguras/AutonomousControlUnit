/*
 * CanRen.cpp

 *
 *  Created on: 09/10/2023
 *      Author: micron
 */
#include "CanFDRen.h"
//#include "../../../../ra/board/board_ra8t1_acuity_bsp/board.h"
//#include "../../../../ra/board/board_ra8t1_acuity_bsp/board_leds.hpp"
//#include "../../../../ra/board/board_ra8t1_acuity_bsp/board_init.hpp"

#include <unordered_map>
#include "utils.h"
#include "../../../Data_structs/Can-Header-Map/CAN_asdb.h"

bool ja_usado = false;
#if defined(BSP_FEATURE_CANFD_FD_SUPPORT) || defined(BSP_FEATURE_CANFD_LITE)

CanFDRen::CanFDRen()  {
    this->g_canfd_ctrl= NULL;
    this->g_canfd_cfg= NULL;
    this->currentCanOpenStack = NULL;

}

CanFDRen::~CanFDRen() {
    this->g_canfd_ctrl= NULL;
    this->g_canfd_cfg= NULL;
}
/**
 * First stage of the CanFDRen Initialization
 * @param _g_canfd_ctrl
 * @param _g_canfd_cfg
 * @return
 */
int CanFDRen::initialization(){
    if(this->initialized){ // @suppress("Ambiguous problem")
        //TODO Evaluate risk
        return FSP_ERR_ALREADY_OPEN;

    }
    uint16_t channel_selected =-1;

    canfd_instance_ctrl_t  * tmp_canfd_ctrl;
    const can_cfg_t * tmp_canfd_cfg;
    if(!ja_usado){
        tmp_canfd_ctrl = &g_canfd0_ctrl;
        tmp_canfd_cfg = &g_canfd0_cfg;

        this->channel=0;
        ja_usado = true;
    }else{
        tmp_canfd_ctrl = &g_canfd1_ctrl;
        tmp_canfd_cfg = &g_canfd1_cfg;
        this->channel=1;
    }

    int ret_init = this->initialization(tmp_canfd_ctrl, tmp_canfd_cfg);
        if(FSP_SUCCESS == ret_init){

            s_canStateKernel(channel_selected, OCCUPIED);

        }
    return ret_init;

}

/**
 * Second stage of the CanFDRen Initialization
 * @param _g_canfd_ctrl
 * @param _g_canfd_cfg
 * @return
 */
int CanFDRen::initialization(canfd_instance_ctrl_t * _g_canfd_ctrl, const can_cfg_t * _g_canfd_cfg){
    this->g_canfd_ctrl = _g_canfd_ctrl;
    this->g_canfd_cfg = _g_canfd_cfg;
    volatile UINT status = R_CANFD_Open(this->g_canfd_ctrl, this->g_canfd_cfg);

    if(FSP_SUCCESS == status){

        tx_ready=true;
        rx_ready= true;
        this->initialized = true; // @suppress("Ambiguous problem")

        if(this->channel ==1){
            //Instantiate the CANopenStack
            this->currentCanOpenStack = new CANopenStack(5);

        }
    }
    return (int)status;

}
/***
 * This function is not yet implemented
 * @param _g_canfd_ctrl
 * @param _g_canfd_cfg
 * @return FSP_SUCCESS
 */


int CanFDRen::channelInjection(canfd_instance_ctrl_t * _g_canfd_ctrl, const can_cfg_t * _g_canfd_cfg){
    FSP_PARAMETER_NOT_USED(_g_canfd_ctrl);
    FSP_PARAMETER_NOT_USED(_g_canfd_cfg);

    return FSP_SUCCESS;
}
uint32_t CanFDRen::close(){
    R_CANFD_Close(this->g_canfd_ctrl);
    return 0;

}
//TODO: This function should be probs split or its focus should be rethinked
bool CanFDRen::checkCanChannelAnyUsed(uint16_t * fetch_channelId){

    std::unordered_map<uint16_t, e_acuity_can_status> kernel_map = g_canStateKernelMap();
	bool ret = false;
    //TODO: maybbe this should be changed to a list or something that lets me lookup for values.
	for(uint16_t z = 0; z > (uint16_t)kernel_map.size(); z++){
		if(kernel_map[z] == AVAILABLE){
		    uint16_t _z=z;
			fetch_channelId = &_z;
			continue;
		}else{

			ret = true;

		}

	}
    return ret;

}
/**
 *
 * @param data @b MUST be already allocated...
 * @param stream_size @b HERE stream_size means how many messages are going to be sent over data default is @b one
 */
uint32_t CanFDRen::recv(void * data, uint32_t stream_size=0){

    return recv(data, CANFD_RX_BUFFER_MB_0, stream_size);
}
uint32_t CanFDRen::recv(void* data, uint32_t buffer=CANFD_RX_BUFFER_FIFO_0, uint32_t stream_size=0){
    FSP_PARAMETER_NOT_USED(stream_size);
    if (this->rx_ready != true)
    {
        return (uint32_t)FSP_ERR_CAN_DATA_UNAVAILABLE;
    }
    uint32_t status = (uint32_t)R_CANFD_Read(this->g_canfd_ctrl, buffer, (can_frame_t *)data);

    return status;
}
/**
 * @
 * @param data
 * @param stream_size
 * @return
 */
uint32_t CanFDRen::write(void *data, uint32_t stream_size){
    FSP_PARAMETER_NOT_USED(stream_size);
	//TODO buffers??!
//	while(this->tx_ready != true) {
//
//	}

 	UINT status = R_CANFD_Write(this->g_canfd_ctrl, 0, (can_frame_t *)data);

	this->tx_ready=false;

	return status;
}
uint32_t CanFDRen::decode(uint32_t buffer){

    can_info_t info = {
         .status  = 0,
         .rx_mb_status = 0,
         .rx_fifo_status = 0,
         .error_count_transmit = 0,
         .error_count_receive = 0,
         .error_code = 0
    };
    do{
        R_CANFD_InfoGet(this->g_canfd_ctrl, &info);
        can_frame_t p_frame;
        R_CANFD_Read(g_canfd_ctrl, buffer, &p_frame);
        decodeImmediate(p_frame);
    }while(info.rx_fifo_status != 1);
    return FSP_SUCCESS;
}
uint32_t CanFDRen::decodeImmediate(can_frame_t frame){
    if(channel >1){
        //TODO its probs not opened
       return FSP_ERR_CAN_INIT_FAILED;
    }
    if (channel == 0){
        //CANFD
    }
    else if (channel == 1){
        //CAN
        switch(frame.id){
            //TODO fix this shyte

            case BOOTUP_ADDRESS_COBID():
                this->currentCanOpenStack->a_bootedNodes(5);
                break;
            case SDO_RESPONSE_ADDRESS_COBID():
                can_frame_stream _data;
                memcpy(&_data, frame.data, 8);
                this->currentCanOpenStack->callback(_data);
                break;

            default:
                break;
        };

    }

    return FSP_SUCCESS;
}
void CanFDRen::callbackHandle(can_callback_args_t *p_args){
    uint32_t buf;
    switch (p_args->event){
        case CAN_EVENT_TX_COMPLETE:

            this->tx_ready=true;
            led_flip(1);
            break;
        case CAN_EVENT_RX_COMPLETE:

            this->rx_ready=true;
            led_flip(2);
            buf=  p_args->buffer;

            decode(buf);
            break;

        case CAN_EVENT_TX_ABORTED:           /* Transmit abort event. */
        case CAN_EVENT_TX_FIFO_EMPTY:       /* Transmit FIFO is empty. */
            //TODO this should be treated as a weird glitch
            this->tx_ready = true;
            break;

        case CAN_EVENT_BUS_RECOVERY:         /* Bus recovery error event */
            this->tx_ready = true;
            this->rx_ready = true;
            break;
        case CAN_EVENT_ERR_WARNING:          /* error warning event */

        case CAN_EVENT_ERR_PASSIVE:          /* error passive event */
            //TODO: log
           break;
        case CAN_EVENT_MAILBOX_MESSAGE_LOST: /* overwrite/overrun error event */
        case CAN_EVENT_FIFO_MESSAGE_LOST:
            //TODO welp we're loosing messages clear the buffer by force!!!
            break;
        case CAN_EVENT_ERR_CHANNEL:          /* Channel error has occurred. */
        case CAN_EVENT_ERR_BUS_OFF:          /* error bus off event */
        case CAN_EVENT_ERR_BUS_LOCK:         /* Bus lock detected (32 consecutive dominant bits). */
        case CAN_EVENT_ERR_GLOBAL:  /* Global error has occurred. */
        {
            this->tx_ready=false;
            this->rx_ready=false;
          break;
        };




	}

}

#endif
