/*
 * UartRen.cpp
 *
 *  Created on: 08/07/2024
 *      Author: micron
 */

#include "./UartRen.h"
namespace hardware_drivers {
UartRen::UartRen(){
    error_counter = 0;
    //TODO abstract bellow there might be more than one uart
    this->g_uart_ctrl = (uart_ctrl_t *)&g_uart0_ctrl;
    this->g_uart_cfg = (const uart_cfg_t * )&g_uart0_cfg;

}

UartRen::~UartRen(){}

int UartRen::initialization(){
    //open uart
    fsp_err_t err;
    if (!this->initialized){
        this->open();
        this->initialized = true;
    }

    err = R_SCI_B_UART_Read(g_uart_ctrl, &it_buffer[0], UART_IT_BUFFER_SIZE);
    //empty buffer
    memset(it_buffer, 0, UART_IT_BUFFER_SIZE);
    mode_async= true;

    return (int)err;

}
uint32_t UartRen::close(){
    fsp_err_t err;
    err = R_SCI_B_UART_Close(g_uart_ctrl);
    return err;

}
uint32_t UartRen::open(){
    fsp_err_t err;
    err = R_SCI_B_UART_Open (g_uart_ctrl, g_uart_cfg);
    return err;
}
//Very fucking nice first compatible protocol without any param changes!!!
uint32_t UartRen::recv(void *data, uint32_t stream_size){
    fsp_err_t err= FSP_ERR_ABORTED;
    size_t wrote = 0;
    uint8_t * _data = (uint8_t *) data;
    if (mode_async){
        it_tail = UART_IT_BUFFER_SIZE - g_uart0_ctrl.rx_dest_bytes;

                    if (it_head != it_tail){
                        while (it_head != it_tail && wrote < stream_size){
                            _data[wrote] = it_buffer[it_head];
                            it_head = (it_head + 1) % UART_IT_BUFFER_SIZE;
                            wrote++;
                        }
                        return FSP_SUCCESS;
                    }


    }else{
        err = R_SCI_B_UART_Read(g_uart_ctrl, &it_buffer[0], stream_size);
        memcpy(data, it_buffer, stream_size);
        return FSP_SUCCESS;
    }
    return err;
}

uint32_t UartRen::write(void *data, uint32_t stream_size){
    fsp_err_t err;

    err = R_SCI_B_UART_Write(g_uart_ctrl, (unsigned char *)data, stream_size);

    return err;
}
void UartRen::user_uart_callback (uart_callback_args_t * p_args)
{
    switch (p_args->event)
    {
        case UART_EVENT_TX_COMPLETE:
            g_write_complete = true;
            break;

        case UART_EVENT_RX_COMPLETE:
            if(mode_async){
                R_SCI_B_UART_Read(&g_uart_ctrl, &it_buffer[0], UART_IT_BUFFER_SIZE);
            }
            break;

        default:
            break;
    }
}


/** UartRenAdapter
 *
 */

UartRenAdapter::UartRenAdapter(){
    uartRen = new UartRen();
}


bool UartRenAdapter::open_handle(struct uxrCustomTransport *transport){
    FSP_PARAMETER_NOT_USED(transport);
    auto k = this->uartRen->initialization();
    if (k == FSP_SUCCESS){
        return true;
    }
    return false;
}
bool UartRenAdapter::close_handle(struct uxrCustomTransport *transport){
    FSP_PARAMETER_NOT_USED(transport);
    auto k = this->uartRen->close ();
    if (k == FSP_SUCCESS)
    {
        return true;
    }
    return false;
}
size_t UartRenAdapter::read_handle(struct uxrCustomTransport *transport, uint8_t *buffer, size_t length, int timeout, uint8_t *error_code){
    int64_t start = uxr_millis();


    while ((uxr_millis() -  start) < timeout){
       this->uartRen->recv(buffer, length);

        R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MICROSECONDS);
    }

    return 0;
}
}//NAMESPACE end

