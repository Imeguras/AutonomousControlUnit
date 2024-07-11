/*
 * MicroRosDuoGen.h
 *
 *  Created on: 08/07/2024
 *      Author: micron
 */
#include "../AbstractPeripheralLayer.cpp"
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rcutils/allocator.h>
#include <rmw_microros/rmw_microros.h>
#include <uxr/client/transport.h>
#include <uxr/client/util/time.h>
#include <rmw_microxrcedds_c/config.h>
#include "../MicroRosBoylerplate/microros_transports.h"
#include "../MicroRosBoylerplate/microros_allocators.h"
#include <typeinfo>
#include "../../../ra/board/ra8t1_acuity_bsp/board.h"
#include "../../../ra/board/ra8t1_acuity_bsp/board_leds.hpp"


#ifndef MICROROSDUOGEN_H_
#define MICROROSDUOGEN_H_

enum transport_type{
    UART_RAW=0,
    ETH_UDP=1,
    ETH_TCP=2,
    TRANSPORT_UNKNOWN
};

template <typename T>
class MicroRosDuoGen : AbstractPeripheralLayer{
public:
    MicroRosDuoGen<T>();
    virtual ~MicroRosDuoGen<T>();
    int initialization();
    uint32_t  error_counter=0;
    uint32_t recv(void * data, uint32_t stream_size);
    uint32_t write(void *data, uint32_t stream_size);

private:
    transport_type transport;
    custom_transport_args remote_addr;

};



class TargetAdapter {
    public:
        virtual ~TargetAdapter()= default;
        virtual bool open_handle(struct uxrCustomTransport* transport);
        virtual bool close_handle(struct uxrCustomTransport* transport);
        virtual size_t write_handle(
                struct uxrCustomTransport* transport,
                const uint8_t* buffer,
                size_t length,
                uint8_t* error_code);

        virtual size_t read_handle(
                struct uxrCustomTransport* transport,
                uint8_t* buffer,
                size_t length,
                int timeout,
                uint8_t* error_code);


};
#endif /* MICROROSDUOGEN_H_ */
