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

#include "HighSpeedAbsL.cpp"
#include "interfaces/EthDuo.h"
#include "interfaces/MicroRosDuo.h"
#include "MicroRosHumble/microros_transports.h"

#include "Data_structs/AutomataStructs.hpp"
#include "Data_structs/Store.h"
#include "utils.h"

#define TX_DATA_HIGH_SPEED_TIMEOUT 32
#define ROS2_EXECUTOR_MAX_HANDLES 2

std_msgs__msg__Int8 msg_incoming;
static std_msgs__msg__Int8 msg_status;
static std_msgs__msg__Int8 msg_mission;
/* New Thread entry function */
void (* sub_callback)(const void *);
void thread_setup(void);
void subscription_callback_status(const void * msgin);
void subscription_callback_mission(const void * msgin);

void high_speed_interface_thread0_entry(void) {
    //pre setup
    thread_setup();

    led_update(red, BSP_IO_LEVEL_HIGH);
	HighSpeed_AbsL<MicroRosDuo> ros;

	led_blink(3);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

	rcl_allocator_t allocator = rcl_get_default_allocator();
	rcl_init_options_t init_options = rcl_get_zero_initialized_init_options();

	auto _opt_init = rcl_init_options_init(&init_options, allocator);
	auto _opt_set_domain =rcl_init_options_set_domain_id(&init_options, 42);
	FSP_PARAMETER_NOT_USED(_opt_init);
	FSP_PARAMETER_NOT_USED(_opt_set_domain);

	// Initialize rclc support object with custom options
	rclc_support_t support;
	rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator);
    rcl_node_t node;

    rclc_executor_t executor=rclc_executor_get_zero_initialized_executor();

    rclc_executor_init(&executor, &support.context, ROS2_EXECUTOR_MAX_HANDLES, &allocator);

    rcl_publisher_t publisher_critical_status;
    rcl_subscription_t subscriber_critical_status= rcl_get_zero_initialized_subscription();
	rcl_publisher_t publisher_critical_mission;
    rcl_subscription_t subscriber_critical_mission= rcl_get_zero_initialized_subscription();
#pragma GCC diagnostic pop


    rcl_ret_t rc = RCL_RET_OK;

    rclc_node_init_default(&node, "faadihgas_node", "", &support);

    rc = rclc_publisher_init_default(&publisher_critical_status,&node,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int8) , "/acu_origin/system_status/critical_as/state");
    if (RCL_RET_OK != rc) {
        error_blink(3);
        return;

    }
    rc = rclc_subscription_init_default(&subscriber_critical_status, &node,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int8), "/pc_origin/system_status/critical_as/state");
    if (RCL_RET_OK != rc) {
        error_blink(3);
        return;
    }
	rc = rclc_publisher_init_default(&publisher_critical_mission,&node,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int8) , "/acu_origin/system_status/critical_as/mission");
    if (RCL_RET_OK != rc) {
        error_blink(3);
        return;

    }
    rc = rclc_subscription_init_default(&subscriber_critical_mission, &node,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int8), "/pc_origin/system_status/critical_as/mission");
    if (RCL_RET_OK != rc) {
        error_blink(3);
        return;
    }
    rc = rclc_executor_add_subscription(&executor, &subscriber_critical_status, &msg_incoming, &subscription_callback_status, ON_NEW_DATA);
	if (RCL_RET_OK != rc) {
         error_blink(3);
         return;
     }
	rc = rclc_executor_add_subscription(&executor, &subscriber_critical_mission, &msg_incoming, &subscription_callback_mission, ON_NEW_DATA);
    if (RCL_RET_OK != rc) {
         error_blink(3);
         return;
     }




    wrapper_int8<critical_as_state> payload = {OFF};
    wrapper_int8<critical_as_state> payload_old = payload;
	wrapper_int8<critical_as_mission> payload_mission = {UNKNOWN_MISSION};
	wrapper_int8<critical_as_mission> payload_mission_old = payload_mission;
    
	led_update(red, BSP_IO_LEVEL_LOW);
    led_update(blue, BSP_IO_LEVEL_HIGH);
	while(1){
	    rclc_executor_spin_some(&executor, 1000*1000);


	    //R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);

	    payload= msg_status;
	    payload_mission= msg_mission;


	    volatile auto publish_ret_status = rcl_publish(&publisher_critical_status, &payload, NULL);
	    volatile auto publish_ret_mission = rcl_publish(&publisher_critical_mission, &payload_mission, NULL);
	    FSP_PARAMETER_NOT_USED(publish_ret_status);
	    FSP_PARAMETER_NOT_USED(publish_ret_mission);


        if(payload!=payload_old || payload_mission!=payload_mission_old){

            tx_semaphore_get(&css, TX_DATA_HIGH_SPEED_TIMEOUT);
                store::Store::getInstance().critical_autonomous_system_status = payload;
				store::Store::getInstance().critical_autonomous_mission_status = payload_mission;


            tx_semaphore_put(&css);
            payload_old=payload;
            payload_mission_old = payload_mission;
        }


	}

}

void subscription_callback_status(const void * msgin){
    const std_msgs__msg__Int8 * msg_cast = (const std_msgs__msg__Int8 *)msgin;
    msg_status= *msg_cast;

    /*led_update(blue, BSP_IO_LEVEL_LOW);
    R_BSP_SoftwareDelay(200, BSP_DELAY_UNITS_MILLISECONDS);
    led_update(blue, BSP_IO_LEVEL_HIGH);*/
}
void subscription_callback_mission(const void * msgin){
    const std_msgs__msg__Int8 * msg_cast = (const std_msgs__msg__Int8 *)msgin;
    msg_mission= *msg_cast;

    /*led_update(blue, BSP_IO_LEVEL_LOW);
    R_BSP_SoftwareDelay(200, BSP_DELAY_UNITS_MILLISECONDS);
    led_update(blue, BSP_IO_LEVEL_HIGH);*/
}

void thread_setup(void){
    tx_semaphore_get(&css, TX_DATA_HIGH_SPEED_TIMEOUT);
        store::Store::getInstance().critical_autonomous_system_status = msg_status;
        store::Store::getInstance().critical_autonomous_mission_status = msg_mission;


    tx_semaphore_put(&css);
}

