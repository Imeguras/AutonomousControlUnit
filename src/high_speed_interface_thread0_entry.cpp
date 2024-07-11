#include <Data_structs/Store.cpp>
#include <high_speed_interface_thread0.h>
#include <string.h>
#include <functional>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int8.h>
#include <list>
#include <map>
#include <iostream>
#include "rmw_microros/rmw_microros.h"

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

#define TX_DATA_HIGH_SPEED_TIMEOUT 32
#define ROS2_EXECUTOR_MAX_HANDLES 2

std_msgs__msg__Int8 msg_incoming;
//static std_msgs__msg__Int8 msg_status;
//static std_msgs__msg__Int8 msg_mission;
/* New Thread entry function */
void (* sub_callback)(const void *);
void thread_setup(void);
void subscription_callback_status(const void * msgin);
void subscription_callback_mission(const void * msgin);
using namespace hardware_drivers;
void high_speed_interface_thread0_entry(void) {

    led_blink(7, 3);
    led_update(0, BSP_IO_LEVEL_HIGH);

    HighSpeed_AbsL<MicroRosDuoGen<UartRenAdapter>> micro_ros;
    rcl_allocator_t allocator = rcl_get_default_allocator();

        rclc_support_t support;

        rcl_init_options_t init_options = rcl_get_zero_initialized_init_options();

        auto _iosdid =rcl_init_options_set_domain_id(&init_options, 42);
        auto _opt_init = rcl_init_options_init(&init_options, allocator);
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
            rcl_publish(&publisher, &msg_output, NULL);
            R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MILLISECONDS);
            tx_thread_sleep (1);
        }

    //HighSpeed_AbsL<MicroRosDuo> eth;
    /*HighSpeed_AbsL<EthDuo> eth;
    if(eth->error_counter > 0){
        //TODO: retry?
        return;
    }*/



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
//
