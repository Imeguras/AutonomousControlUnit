/*
 * MicroRosDuoGen.cpp
 *
 *  Created on: 08/07/2024
 *      Author: micron
 */

#include "./MicroRosDuoGen.h"

using namespace hardware_drivers;
template <typename T>
inline MicroRosDuoGen<T>::MicroRosDuoGen(){
    //check if template is EthDuo
    if (typeid(T) == typeid(EthDuo)){
        this->transport = ETH_UDP;
    } else if (typeid(T) == typeid(hardware_drivers::UartRenAdapter)){
        this->transport = UART_RAW;
    } else {
        this->transport = TRANSPORT_UNKNOWN;
    }


}

template <typename T>
inline int MicroRosDuoGen<T>::initialization(){
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

            rmw_uros_set_custom_transport(true,
                                          NULL,
                                          driver.open_handle,
                                          driver.close_handle,
                                          driver.write,
                                          driver.recv);
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

    int ret_init = driver.initialization();
    if(ret_init){

        return ret_init;
    }
    return -1;
}
template <typename T>
inline uint32_t MicroRosDuoGen<T>::recv(void * data, uint32_t stream_size){
    return 0;
}
template <typename T>
inline uint32_t MicroRosDuoGen<T>::write(void *data, uint32_t stream_size){
    return 0;

}


