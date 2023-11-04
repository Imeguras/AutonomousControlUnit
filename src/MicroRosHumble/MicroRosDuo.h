/*
 * MicroRosDuo.h
 *
 *  Created on: 02/11/2023
 *      Author: micron
 */

#include "MicroRosHumble/EthDuo.h"

#include <uxr/client/transport.h>
#include <rmw_microros/rmw_microros.h>
#include <rmw_microxrcedds_c/config.h>
#include "microros_transports.h"
/*
#include "rmw_microros/rmw_microros.h"
#include "microros_transports.h"*/

//../micro_ros_renesas2estudio_component/libmicroros/include/

//#include <uxr/client/transport.h>
//#include <rmw_microxrcedds_c/config.h>
//#include "rmw_microros/rmw_microros.h"
#ifndef MICROROSHUMBLE_MICROROSDUO_H_
#define MICROROSHUMBLE_MICROROSDUO_H_

#define LINK_ENABLE_WAIT_TIME (TX_WAIT_FOREVER)
#define SOCKET_FIFO_SIZE G_PACKET_POOL0_PACKET_NUM
#define TX_MS_TO_TICKS(milliseconds) ((ULONG) ((milliseconds / 1000.0) * TX_TIMER_TICKS_PER_SECOND))

class MicroRosDuo: public EthDuo {
public:
	MicroRosDuo();
	virtual ~MicroRosDuo();
	int initialization() override;

	void* recv(void * data, uint32_t stream_size) override;
	uint32_t	write(void *data, uint32_t stream_size) override;


	bool _transport_open(struct uxrCustomTransport * transport);
	bool _transport_close(struct uxrCustomTransport * transport);
	size_t _transport_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* err);
	size_t _transport_write(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * err);
protected:
	NX_UDP_SOCKET socket;
	custom_transport_args remote_addr;


};

#endif /* MICROROSHUMBLE_MICROROSDUO_H_ */
