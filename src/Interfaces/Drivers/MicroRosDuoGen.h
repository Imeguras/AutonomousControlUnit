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
#include "Interfaces/Drivers/HardwareBased/EthDuo.h"
#include "Interfaces/Drivers/HardwareBased/UartRen.h"


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
    uint32_t  error_counter=0;
    MicroRosDuoGen<T>(){
        //check if template is EthDuo
        if (typeid(T) == typeid(EthDuo)){
            this->transport = ETH_UDP;
        } else if (typeid(T) == typeid(hardware_drivers::UartRenAdapter)){
            this->transport = UART_RAW;
        } else {
            this->transport = TRANSPORT_UNKNOWN;
        }
    }
    virtual ~MicroRosDuoGen<T>()= default;
    int initialization(){
        using Driver = T;
            Driver driver = Driver();


            switch(this->transport){

                case ETH_UDP:
                case ETH_TCP:
                    this->remote_addr = custom_transport_args{

                                                   .agent_ip_address=IP_ADDRESS(192,168,1,100),
                                                   .agent_port=8888
                                                };

                    //TODO FIX EVERYTHING
        /*            rmw_uros_set_custom_transport(
                                           false,
                                           (void *) &remote_addr,
                                           ,
                                           RosIntanceSingleton::getInstance().handle_close,
                                           RosIntanceSingleton::getInstance().handle_write,
                                           RosIntanceSingleton::getInstance().handle_read);
                                           */

                    break;
                case UART_RAW:

                    /*rmw_uros_set_custom_transport(true,
                                                  NULL,
                                                  driver.open_handle,
                                                  driver.close_handle,
                                                  driver.write_handle,
                                                  driver.read_handle);*/
                   break;
                case TRANSPORT_UNKNOWN:
                default:
                    return -1;
                    break;
            }
            rcl_allocator_t custom_allocator = rcutils_get_zero_initialized_allocator();
            custom_allocator.allocate = microros_allocate;
            custom_allocator.deallocate = microros_deallocate;
            custom_allocator.reallocate = microros_reallocate;
            custom_allocator.zero_allocate =  microros_zero_allocate;
            if (!rcutils_set_default_allocator(&custom_allocator)) {
                //TODO this should be written
                led_blink(0,12);
            }

            //int ret_init = driver.();
//            if(ret_init){
//
//                return ret_init;
//            }
            return -1;
    }

    uint32_t recv(void * data, uint32_t stream_size){
        return 0;
    }
    uint32_t write(void *data, uint32_t stream_size){
        return 0;
    }

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
