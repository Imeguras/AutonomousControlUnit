/*
 * CanRen.h

 *
 *  Created on: 09/10/2023
 *      Author: micron
 */
#include "low_speed_interface_thread0.h"
#include <list>
#include "AbstractPeripheralLayer.cpp"
#include "bsp_api.h"
#ifdef BOARD_RA6M5_EK

#include "common_data.h"
#define CANFDREN_LOOPBACK_TIMEOUT 200
#ifndef CANFDREN_H_
#define CANFDREN_H_
class CanFDRen : AbstractPeripheralLayer{
public:
	CanFDRen();
	virtual ~CanFDRen();
	int initialization() override;
	void* recv(void * data, uint32_t stream_size) override;
	uint32_t	write(void *data, uint32_t stream_size) override;
	volatile bool rx_ready;
	volatile bool tx_ready;
	void canfd_callback(can_callback_args_t *p_args);

private:

	std::list<can_frame_t> buffered_mails_rx;

};


#endif /* CANREN_H_ */
#endif
