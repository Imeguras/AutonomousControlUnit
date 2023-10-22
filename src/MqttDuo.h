/*
 * MqttDuo.h
 *
 *  Created on: 11/10/2023
 *      Author: micron
 */

#ifndef MQTTDUO_H_
#define MQTTDUO_H_

#include <EthDuo.h>

class MqttDuo: public EthDuo {
public:
	MqttDuo();
	virtual ~MqttDuo();
	int initialization();
	void* recv(void * data, uint32_t stream_size);
	uint32_t	write(void *data, uint32_t stream_size);
};

#endif /* MQTTDUO_H_ */
