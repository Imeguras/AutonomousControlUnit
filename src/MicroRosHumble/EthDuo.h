/*
 * ethernet_logic.cpp
 *
 *  Created on: 30/09/2023
 *      Author: João Vieira
 */

#include "bsp_api.h"
#include "common_data.h"
#include "AbstractPeripheralLayer.cpp"
#include <stdint.h>
#include "nxd_dhcp_client.h"
#ifndef ETHDUO_H_
#define ETHDUO_H_

class EthDuo : AbstractPeripheralLayer {
	public:
		  EthDuo();

		  virtual ~EthDuo();
		  int initialization() override;
		  void* recv(void * data, uint32_t stream_size) override;
		  uint32_t	write(void *data, uint32_t stream_size) override;
		  UINT error_counter=0;
	protected:
		  /* Stack memory for g_ip0. */
		  NX_PACKET_POOL g_packet_pool0;
		  /* IP instance */
		  NX_IP g_ip0;
		  NX_DHCP g_dhcp_client0;

		  /* Stack memory for g_ip0. */
		  void g_ip0_quick_setup();
		  void g_packet_pool0_quick_setup();

		  /* Packet pool instance (If this is a Trustzone part, the memory must be
		   * placed in Non-secure memory). */

};

#endif /* ETHDUO_H_ */
