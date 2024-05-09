/*
 * CanRen.cpp

 *
 *  Created on: 09/10/2023
 *      Author: micron
 */
#include "CanFDRen.h"

//#if defined(BSP_FEATURE_CANFD_FD_SUPPORT) || defined(BSP_FEATURE_CANFD_LITE)



CanFDRen::CanFDRen()  {

}

CanFDRen::~CanFDRen() {

}
//By default go to channel 0
int CanFDRen::initialization(){
    //this->g_canfd_ctrl = g_canfd0_ctrl;
    //this->g_canfd_cfg = g_canfd0_cfg;

    UINT status = R_CANFD_Open(&g_canfd0_ctrl, &g_canfd0_cfg);
    if(status== FSP_SUCCESS){
        tx_ready=true;
        rx_ready= true;
    }
    return (int)status;
}

int CanFDRen::initialization(canfd_instance_ctrl_t& _g_canfd_ctrl, can_cfg_t& _g_canfd_cfg){
    this->g_canfd_ctrl = _g_canfd_ctrl;
    this->g_canfd_cfg = _g_canfd_cfg;
	UINT status = R_CANFD_Open(&g_canfd_ctrl, &g_canfd_cfg);
	if(status== FSP_SUCCESS){
		tx_ready=true;
		rx_ready= true;
	}
	return (int)status;
}
/**
 *
 * @param data @b MUST be already allocated...
 * @param stream_size @b HERE stream_size means how many messages are going to be sent over data default is @b one
 */
void* CanFDRen::recv(void * data, uint32_t stream_size=1){
    FSP_PARAMETER_NOT_USED(stream_size);
	auto index = this->fbuffers_rx.front();
	this->fbuffers_rx.pop_front();
	//TODO: read more than one?

	R_CANFD_Read(&this->g_canfd_ctrl,index,  (can_frame_t *)&data);
	return ((void *)data);
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
	/*while(this->tx_ready != true) {

	}*/

 	UINT status = R_CANFD_Write(&this->g_canfd_ctrl, 0, (can_frame_t *)data);

	//TODO: multiple tx
	this->tx_ready=false;

	return status;
}

void CanFDRen::callbackHandle(can_callback_args_t *p_args){
    led_update(lime, BSP_IO_LEVEL_HIGH);
	switch (p_args->event){
        case CAN_EVENT_TX_COMPLETE:
            led_update(ambar, BSP_IO_LEVEL_HIGH);
            this->tx_ready=true;
            R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
            led_update(ambar, BSP_IO_LEVEL_LOW);
            break;
        case CAN_EVENT_RX_COMPLETE:
            //TODO validations?
            this->fbuffers_rx.push_back(p_args->buffer);
            this->rx_ready=true;
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
        case CAN_EVENT_TX_FIFO_EMPTY:       /* Transmit FIFO is empty. */
        case CAN_EVENT_FIFO_MESSAGE_LOST:
        {
            this->tx_ready=false;
            this->rx_ready=false;
          break;
        }

	}

	R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
    led_update(lime, BSP_IO_LEVEL_LOW);
}

//#endif
