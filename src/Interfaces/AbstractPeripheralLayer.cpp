/*
 * AbstractPeripheralLayer.cpp
 *
 *  Created on: 09/10/2023
 *      Author: micron
 */

#include "common_data.h"
#include "hal_data.h"
#include <stdint.h>
#pragma once
//TODO: era bom que isto fosse para o .h(linker+template issues)
class AbstractPeripheralLayer {
    public:
        AbstractPeripheralLayer(){

        }
        virtual ~AbstractPeripheralLayer(){

        }

        uint32_t  error_counter=0;
        virtual int initialization() = 0;
        virtual uint32_t recv(void * data, uint32_t stream_size) = 0;
        virtual uint32_t write(void *data, uint32_t stream_size) = 0;
    protected:
        bool initialized=false;
};
