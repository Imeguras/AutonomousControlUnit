/*
 * CanRen.h

 *
 *  Created on: 09/10/2023
 *      Author: micron
 */
#include "bsp_api.h"
#include "common_data.h"
#if BSP_FEATURE_CAN_NUM_CHANNELS >= 1
#include "low_speed_interface_thread0.h"
#include <list>
#include "../../AbstractPeripheralLayer.cpp"

#define CANREN_LOOPBACK_TIMEOUT 200
#ifndef CANREN_H_
#define CANREN_H_
class CanRen : AbstractPeripheralLayer{
public:
	CanRen();
	virtual ~CanRen();
	int initialization() override;
	void* recv(void * data, uint32_t stream_size) override;
	uint32_t	write(void *data, uint32_t stream_size) override;
	volatile bool rx_ready;
	volatile bool tx_ready;
	void can_callback(can_callback_args_t *p_args);
	/*uint32_t g_MailboxAdress();
	void s_MailboxAddress( uint32_t address);*/
private:

	std::list<can_frame_t> buffered_mails_rx;
	std::list<int> available_can_mailboxes_tx;
	std::list<int> available_can_mailboxes_rx;
};


#endif /* CANREN_H_ */
#endif
