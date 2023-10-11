/*
 * CanRen.h
 *
 *  Created on: 09/10/2023
 *      Author: micron
 */
#include "AbstractPeripheralLayer.cpp"
#include "hal_data.h"
#include "bsp_api.h"
#include "common_data.h"
#ifndef CANREN_H_
#define CANREN_H_
class CanRen : AbstractPeripheralLayer{
public:
	CanRen();
	virtual ~CanRen();
	int initialization() override;
	void* recv(void * data, uint32_t stream_size) override;
	uint32_t	write(void *data, uint32_t stream_size) override;
	/*uint32_t g_MailboxAdress();
	void s_MailboxAddress( uint32_t address);*/
private:
	void can_callback(can_callback_args_t *p_args);
	uint32_t mailbox_address;
};


#endif /* CANREN_H_ */
