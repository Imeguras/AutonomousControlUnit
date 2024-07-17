/*
 * UartRen.h
 *
 *  Created on: 08/07/2024
 *      Author: micron
 */
#include "../../AbstractPeripheralLayer.cpp"
#include "r_sci_b_uart.h"
#include "r_uart_api.h"
//TODO maybe a injection should be better in this case
#include "high_speed_interface_thread0.h"
#include <uxr/client/transport.h>
#include <uxr/client/util/time.h>
#include <rmw_microxrcedds_c/config.h>
#include "rmw_microros/rmw_microros.h"

#include <stddef.h>
#ifndef INTERFACES_DRIVERS_HARDWAREBASED_UARTREN_H_
#define INTERFACES_DRIVERS_HARDWAREBASED_UARTREN_H_
#define UART_IT_BUFFER_SIZE 2048
namespace hardware_drivers {
    class UartRen : AbstractPeripheralLayer {
        public:
            UartRen();
            virtual ~UartRen()=default;
            int initialization();
            uint32_t recv(void *data, uint32_t stream_size);
            uint32_t write(void *data, uint32_t stream_size);
            uint32_t close();
            uint32_t open();
            uint32_t error_counter = 0;
            bool mode_async = true;
            volatile bool g_write_complete = false;




            void user_uart_callback (uart_callback_args_t * p_args);
            uint8_t it_buffer[UART_IT_BUFFER_SIZE];
            uart_ctrl_t * g_uart_ctrl;
            const uart_cfg_t * g_uart_cfg;
        protected:
            volatile size_t it_head = 0;
            volatile size_t it_tail = 0;

                        bool initialized = false;


    };
//: public TargetAdapter
    class UartRenAdapter {
        public:
            UartRenAdapter();
            virtual ~UartRenAdapter()=default;
            bool open_handle(struct uxrCustomTransport *transport);
            bool close_handle(struct uxrCustomTransport *transport);
            size_t write_handle(struct uxrCustomTransport *transport, const uint8_t *buffer, size_t length,
                    uint8_t *error_code);
            size_t read_handle(struct uxrCustomTransport *transport, uint8_t *buffer, size_t length, int timeout,
                    uint8_t *error_code);
            UartRen *uartRen=NULL;



    };
}
#endif /* INTERFACES_DRIVERS_HARDWAREBASED_UARTREN_H_ */
