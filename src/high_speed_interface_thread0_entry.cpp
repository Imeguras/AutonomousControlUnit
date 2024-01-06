#include <high_speed_interface_thread0.h>
#include <string.h>
#include <functional>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int8.h>

#include "rmw_microros/rmw_microros.h"

#include "HighSpeedAbsL.cpp"
#include "interfaces/EthDuo.h"
#include "interfaces/MicroRosDuo.h"
#include "MicroRosHumble/microros_transports.h"

#include "data_structs/AutomataStructs.hpp"
#include "data_structs/Store.h"
#include "utils.h"
#define TX_DATA_HIGH_SPEED_TIMEOUT 32
#define ROS2_EXECUTOR_MAX_HANDLES 1
static std_msgs__msg__Int8 msg;
/* New Thread entry function */
void (* sub_callback)(const void *);
void subscription_callback(const void * msgin);
// Implementation example:
void error_blink(int f);
void high_speed_interface_thread0_entry(void) {
    led_update(red, BSP_IO_LEVEL_HIGH);
	HighSpeed_AbsL<MicroRosDuo> ros;
	for(int i = 3; i > 0; i--){
	    led_update(green,BSP_IO_LEVEL_HIGH );
	    R_BSP_SoftwareDelay(500,BSP_DELAY_UNITS_MILLISECONDS );
        led_update(green,BSP_IO_LEVEL_LOW );
        R_BSP_SoftwareDelay(500,BSP_DELAY_UNITS_MILLISECONDS );
	}


	rcl_allocator_t allocator = rcl_get_default_allocator();
	rcl_init_options_t init_options = rcl_get_zero_initialized_init_options();
	auto _oi = rcl_init_options_init(&init_options, allocator);
	auto _iosdid =rcl_init_options_set_domain_id(&init_options, 42);
	FSP_PARAMETER_NOT_USED(_oi);
	FSP_PARAMETER_NOT_USED(_iosdid);
	// Initialize rclc support object with custom options
	rclc_support_t support;
	rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator);
    rcl_node_t node;

    rclc_executor_t executor=rclc_executor_get_zero_initialized_executor();

    rclc_executor_init(&executor, &support.context, ROS2_EXECUTOR_MAX_HANDLES, &allocator);

    rcl_publisher_t publisher_critical_status;
    rcl_subscription_t subscriber_critical_status= rcl_get_zero_initialized_subscription();


    rcl_ret_t rc = RCL_RET_OK;
    const unsigned int timer_period = RCL_MS_TO_NS(1000);



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

    rc = rclc_executor_add_subscription(
      &executor, &subscriber_critical_status, &msg,
      &subscription_callback, ON_NEW_DATA);

    if (RCL_RET_OK != rc) {
         error_blink(3);
         return;
     }




    wrapper_int8<critical_as_state> payload = {OFF};
    wrapper_int8<critical_as_state> payload_old = payload;

    led_update(red, BSP_IO_LEVEL_LOW);
    led_update(blue, BSP_IO_LEVEL_HIGH);
	while(1){

	    rclc_executor_spin_some(&executor, 1000*1000);

	    //R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);

	    payload = msg;
	    auto publish_ret = rcl_publish(&publisher_critical_status, &payload, NULL);
	    FSP_PARAMETER_NOT_USED(publish_ret);

        if(payload!=payload_old ){
            tx_semaphore_get(&css, TX_DATA_HIGH_SPEED_TIMEOUT);
                store::critical_autonomous_system_status = payload;
            tx_semaphore_put(&css);
            payload_old=payload;
        }


	}

}
void error_blink(int f){
    for(int i = f; i > 0; i--){
        led_update(red,BSP_IO_LEVEL_LOW );
        R_BSP_SoftwareDelay(500,BSP_DELAY_UNITS_MILLISECONDS );
        led_update(red,BSP_IO_LEVEL_HIGH );
        R_BSP_SoftwareDelay(500,BSP_DELAY_UNITS_MILLISECONDS );
    }
}
void subscription_callback(const void * msgin){
    led_update(blue, BSP_IO_LEVEL_LOW);
    R_BSP_SoftwareDelay(200, BSP_DELAY_UNITS_MILLISECONDS);
    led_update(blue, BSP_IO_LEVEL_HIGH);
  // Cast received message to used type

    const std_msgs__msg__Int8 * msg_cast = (const std_msgs__msg__Int8 *)msgin;
    msg.data = msg_cast->data;

}

/*


    	R_BSP_SoftwareDelay(10,BSP_DELAY_UNITS_MILLISECONDS);
	}
}


*/

