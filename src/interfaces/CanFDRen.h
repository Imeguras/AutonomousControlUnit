/*
 * CanRen.h


 *
 *  Created on: 09/10/2023
 *      Author: micron
 */
#include <list>
#include "low_speed_interface_thread0.h"
#include "AbstractPeripheralLayer.cpp"
#include "bsp_api.h"
#include "common_data.h"

#if BSP_FEATURE_CANFD_FD_SUPPORT


#define CANFDREN_LOOPBACK_TIMEOUT 200
#ifndef CANFDREN_H_
#define CANFDREN_H_

#if not defined(__IGNORE_CANFD_USAGE__) and BSP_FEATURE_CANFD_NUM_INSTANCES == 0
    #error "You need to add a CANFD stack in order to use this"
#endif

class CanFDRen : AbstractPeripheralLayer{
public:
	CanFDRen();
	virtual ~CanFDRen();
	int initialization() override;
	void* recv(void * data, uint32_t stream_size) override;
	uint32_t	write(void *data, uint32_t stream_size) override;
	volatile bool rx_ready;
	volatile bool tx_ready;

	void callbackHandle(can_callback_args_t *p_args);
private:
	std::list<uint32_t> fbuffers_rx;


};

#endif /* CANREN_H_ */

#endif
