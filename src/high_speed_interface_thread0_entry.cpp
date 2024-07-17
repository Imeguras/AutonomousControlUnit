#include <Data_structs/Store.cpp>
#include <high_speed_interface_thread0.h>
#include <string.h>
#include <functional>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int8.h>
#include <lart_msgs/msg/state.h>
#include <list>
#include <map>
#include <iostream>
#include "rmw_microros/rmw_microros.h"
#include <lart_msgs/msg/as_status.h>
#include "Interfaces/HighSpeedAbsL.cpp"
#include "Interfaces/Drivers/HardwareBased/EthDuo.h"
#include "Interfaces/Drivers/HardwareBased/UartRen.h"

#include "Interfaces/Drivers/MicroRosDuo.h"
#include "Interfaces/Drivers/MicroRosDuoGen.h"
#include "Interfaces/MicroRosBoylerplate/microros_transports.h"
//TODO fix this mess
#include "../../../ra/board/ra8t1_acuity_bsp/board.h"
#include "../../../ra/board/ra8t1_acuity_bsp/board_leds.hpp"

#include "Data_structs/AutomataStructs.hpp"

#include "utils.h"
#include <functional>


#define TX_DATA_HIGH_SPEED_TIMEOUT 32
#define ROS2_EXECUTOR_MAX_HANDLES 2
extern "C" void user_uart_callback (uart_callback_args_t * p_args);
hardware_drivers::UartRen * interface_callback_uart_t;
std_msgs__msg__Int8 msg_incoming;
//static std_msgs__msg__Int8 msg_status;
//static std_msgs__msg__Int8 msg_mission;
/* New Thread entry function */
void (* sub_callback)(const void *);
void thread_setup(void);
void subscription_callback_status(const void * msgin);
void subscription_callback_mission(const void * msgin);

bool renesas_e2_transport_open(struct uxrCustomTransport * transport);
bool renesas_e2_transport_close(struct uxrCustomTransport * transport);
size_t renesas_e2_transport_write(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * error);
size_t renesas_e2_transport_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* error);
static uint8_t it_buffer[UART_IT_BUFFER_SIZE];
static size_t it_head = 0, it_tail = 0;
bool g_write_complete = false;
hardware_drivers::UartRenAdapter * etc;

using namespace hardware_drivers;
void high_speed_interface_thread0_entry(void) {

    led_blink(7, 3);
    led_update(0, BSP_IO_LEVEL_HIGH);

    //HighSpeed_AbsL<MicroRosDuoGen<UartRenAdapter>> micro_ros;

    rmw_uros_set_custom_transport(
                               true,
                               (void *) NULL,
                               renesas_e2_transport_open,
                               renesas_e2_transport_close,
                               renesas_e2_transport_write,
                               renesas_e2_transport_read);

        rcl_allocator_t custom_allocator = rcutils_get_zero_initialized_allocator();
        custom_allocator.allocate = microros_allocate;
        custom_allocator.deallocate = microros_deallocate;
        custom_allocator.reallocate = microros_reallocate;
        custom_allocator.zero_allocate =  microros_zero_allocate;
        if (!rcutils_set_default_allocator(&custom_allocator)) {
            //TODO this should be written
            led_blink(0,12);
        }


        rcl_allocator_t allocator = rcl_get_default_allocator();

        rclc_support_t support;

        rcl_init_options_t init_options = rcl_get_zero_initialized_init_options();


        auto _opt_init = rcl_init_options_init(&init_options, allocator);
        auto _iosdid =rcl_init_options_set_domain_id(&init_options, 42);
        //auto _retttt = rcl_init_options_fini(&init_options);
        FSP_PARAMETER_NOT_USED(_iosdid);
        FSP_PARAMETER_NOT_USED(_opt_init);
        //auto _ret = rclc_support_init(&support, 0, NULL, &allocator);

        auto _ret = rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator);
        if (_ret == RCL_RET_ERROR){
            //TODO this should be written
            led_blink (0, 12);
        }
        rcl_node_t node ;

    //    rclc_executor_t executor=rclc_executor_get_zero_initialized_executor();
    //
    //    rclc_executor_init(&executor, &support.context, ROS2_EXECUTOR_MAX_HANDLES, &allocator);
        _ret = rclc_node_init_default(&node, "faadihgas_node", "", &support);
        if (_ret == RCL_RET_ERROR)
        {
            led_blink (0, 12);
        }
        rcl_publisher_t publisher;
        rclc_publisher_init_default(&publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int8), "topic");
        std_msgs__msg__Int8 msg_output;

        msg_output.data = 0;

    //    rclc_executor_spin(&executor);


        led_blink(0,3);
        while (1){
            //publish
            msg_output.data=1;
            auto _pub_ret = rcl_publish(&publisher, &msg_output, NULL);
            FSP_PARAMETER_NOT_USED(_pub_ret);
            R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MILLISECONDS);
            tx_thread_sleep (1);
        }

}
extern "C" void user_uart_callback (uart_callback_args_t * p_args){
//    if(interface_callback_uart_t != NULL){
//        interface_callback_uart_t->user_uart_callback(p_args);
//    }
    switch (p_args->event)
    {
        case UART_EVENT_TX_COMPLETE:
            etc->uartRen->g_write_complete = true;
            break;

        case UART_EVENT_RX_COMPLETE:

                R_SCI_B_UART_Read(&etc->uartRen->g_uart_ctrl, &etc->uartRen->it_buffer[0], UART_IT_BUFFER_SIZE);

            break;

        default:
            break;
    }
}

//    rcl_allocator_t allocator = rcl_get_default_allocator();
//    rclc_support_t support;
//    rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator);
//    rcl_node_t node;
//
//    rclc_executor_t executor=rclc_executor_get_zero_initialized_executor();
//
//    rclc_executor_init(&executor, &support.context, ROS2_EXECUTOR_MAX_HANDLES, &allocator);
//
//    rcl_publisher_t publisher_critical_status;
//    rcl_subscription_t subscriber_critical_status;//#include "../ra/board/ra8t1_acuity_bsp/board_leds.hpp"
//
//
//    rcl_ret_t rc = RCL_RET_OK;
//
//    rclc_node_init_default(&node, "faadihgas_node", "", &support);
//
//    rc = rclc_publisher_init_default(&publisher_critical_status,&node,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int8) , "/acu_origin/system_status/critical_as/state");
//    if (RCL_RET_OK != rc) {
//        error_blink(3);
//        return;
//
//    rcl_ret_t rc = rclc_executor_add_subscription(
//      &executor, &subscriber_critical_status, &call_msg,
//      &subscription_callback, ON_NEW_DATA);
//    rclc_executor_spin(&executor);
//    (void) rc;
//    critical_as_int8<critical_as_state> temp_payload = {OFF};
//    critical_as_int8<critical_as_state> temp_payload_old = temp_payload;
//
//    while(1){
//        R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
//
//        //temp_payload = msg;
//        std_msgs__msg__Int8 payload = temp_payload.get();
//        auto publish_ret = rcl_publish(&publisher_critical_status, &payload, NULL);
//        (void) publish_ret;
//
//    }
//
//        payload= msg_status;
//        payload_mission= msg_mission;
//
//
//        volatile auto publish_ret_status = rcl_publish(&publisher_critical_status, &payload, NULL);
//        volatile auto publish_ret_mission = rcl_publish(&publisher_critical_mission, &payload_mission, NULL);
//        FSP_PARAMETER_NOT_USED(publish_ret_status);
//        FSP_PARAMETER_NOT_USED(publish_ret_mission);
//
//    volatile UINT k=0;
//    k=ros->error_counter;
//
//        if(payload!=payload_old || payload_mission!=payload_mission_old){
//
//            tx_semaphore_get(&css, TX_DATA_HIGH_SPEED_TIMEOUT);
//                store::Store::getInstance().critical_autonomous_system_status = payload;
//                store::Store::getInstance().critical_autonomous_mission_status = payload_mission;
//
//        auto publish_ret = rcl_publish(&publisher_critical_status, &msg, NULL);
//        (void) publish_ret;
//        if(temp_payload.state!=temp_payload_old.state ){
//                tx_semaphore_get(&css, TX_DATA_HIGH_SPEED_TIMEOUT);
//                    store::critical_autonomous_system_status = temp_payload;
//                tx_semaphore_put(&css);
//
//            temp_payload_old = temp_payload;
//
//        }
//
//        R_BSP_SoftwareDelay(10,BSP_DELAY_UNITS_MILLISECONDS);
//    }
//}
//
//void subscription_callback_status(const void * msgin){
//    const std_msgs__msg__Int8 * msg_cast = (const std_msgs__msg__Int8 *)msgin;
//    msg_status= *msg_cast;
//
//    led_update(blue, BSP_IO_LEVEL_LOW);
//    R_BSP_SoftwareDelay(200, BSP_DELAY_UNITS_MILLISECONDS);
//    led_update(blue, BSP_IO_LEVEL_HIGH);
//}
//void subscription_callback_mission(const void * msgin){
//    const std_msgs__msg__Int8 * msg_cast = (const std_msgs__msg__Int8 *)msgin;
//    msg_mission= *msg_cast;
//
//    led_update(blue, BSP_IO_LEVEL_LOW);
//    R_BSP_SoftwareDelay(200, BSP_DELAY_UNITS_MILLISECONDS);
//    led_update(blue, BSP_IO_LEVEL_HIGH);
//}
//
//void thread_setup(void){
//    tx_semaphore_get(&css, TX_DATA_HIGH_SPEED_TIMEOUT);
//        store::Store::getInstance().critical_autonomous_system_status = msg_status;
//        store::Store::getInstance().critical_autonomous_mission_status = msg_mission;
//
//
//    tx_semaphore_put(&css);
//}
bool renesas_e2_transport_open(struct uxrCustomTransport * transport){
    (void) transport;
    fsp_err_t err;

#if defined (BOARD_RA6T2_MCK)
    R_SCI_B_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
    err = R_SCI_B_UART_Read(&g_uart0_ctrl, &it_buffer[0], UART_IT_BUFFER_SIZE);
#else
    R_SCI_B_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
    err = R_SCI_B_UART_Read(&g_uart0_ctrl, &it_buffer[0], UART_IT_BUFFER_SIZE);
#endif

    return err == FSP_SUCCESS;
}

bool renesas_e2_transport_close(struct uxrCustomTransport * transport){
    (void) transport;
    fsp_err_t err;

#if defined (BOARD_RA6T2_MCK)
    err = R_SCI_B_UART_Close(&g_uart0_ctrl);
#else
    err = R_SCI_B_UART_Close(&g_uart0_ctrl);
#endif

    return err == FSP_SUCCESS;
}

size_t renesas_e2_transport_write(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * error){
    (void) transport;
    (void) error;

    g_write_complete = false;
    fsp_err_t err;

#if defined (BOARD_RA6T2_MCK)
    err = R_SCI_B_UART_Write(&g_uart0_ctrl, buf, len);
#else
    err = R_SCI_B_UART_Write(&g_uart0_ctrl, buf, len);
#endif

    if (err != FSP_SUCCESS)
    {
        return 0;
    }

    int64_t start = uxr_millis();
    while(!g_write_complete && (uxr_millis() -  start) < WRITE_TIMEOUT)
    {
        R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MICROSECONDS);
    }

    return len;
}

size_t renesas_e2_transport_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* error){
    (void) transport;
    (void) error;

    int64_t start = uxr_millis();
    size_t wrote = 0;

    while ((uxr_millis() -  start) < timeout)
    {
        it_tail = UART_IT_BUFFER_SIZE - g_uart0_ctrl.rx_dest_bytes;

        if (it_head != it_tail)
        {
            while (it_head != it_tail && wrote < len)
            {
                buf[wrote] = it_buffer[it_head];
                it_head = (it_head + 1) % UART_IT_BUFFER_SIZE;
                wrote++;
            }

            break;
        }

        R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MICROSECONDS);
    }

    return wrote;
}
