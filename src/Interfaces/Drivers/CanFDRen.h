/*
 * CanRen.h


 *
 *  Created on: 09/10/2023
 *      Author: micron
 */

//#if defined(BSP_FEATURE_CANFD_FD_SUPPORT) || defined(BSP_FEATURE_CANFD_LITE)
#include <list>
#include "low_speed_interface_thread0.h"
#include "../AbstractPeripheralLayer.cpp"
#include "../ra/board/ra8t1_acuity_bsp/board_leds.hpp"
#include "bsp_api.h"
#include "common_data.h"
#define CANFDREN_LOOPBACK_TIMEOUT 200
    #ifndef CANFDREN_H_
    #define CANFDREN_H_
class CanFDRen : AbstractPeripheralLayer{
public:
	CanFDRen();
	virtual ~CanFDRen();
	int initialization() override;
	int initialization(canfd_instance_ctrl_t& _g_canfd_ctrl, can_cfg_t& _g_canfd_cfg) ;
	void* recv(void * data, uint32_t stream_size) override;
	uint32_t	write(void *data, uint32_t stream_size) override;
	volatile bool rx_ready;
	volatile bool tx_ready;

	void callbackHandle(can_callback_args_t *p_args);
private:
	std::list<uint32_t> fbuffers_rx;
	//TODO: make channel 0 "non mandatory"
	canfd_instance_ctrl_t& g_canfd_ctrl = (canfd_instance_ctrl_t&)g_canfd0_ctrl;
	can_cfg_t& g_canfd_cfg = (can_cfg_t&)g_canfd0_cfg;

};


    #endif /* CANREN_H_ */
//#endif
