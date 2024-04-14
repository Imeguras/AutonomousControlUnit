/*
 * AbstractPeripheralLayer.cpp
 *
 *  Created on: 09/10/2023
 *      Author: micron
 */

#include "common_data.h"
#pragma once
class AbstractPeripheralLayer {
public:
	AbstractPeripheralLayer(){

	}
	virtual ~AbstractPeripheralLayer(){

	}

	UINT  error_counter=0;
	virtual int initialization() = 0;
	virtual void* recv(void * data, uint32_t stream_size) = 0;
	virtual uint32_t	write(void *data, uint32_t stream_size) = 0;
};
