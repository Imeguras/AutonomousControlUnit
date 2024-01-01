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
static std_msgs__msg__Int8 msg;
std_msgs__msg__Int8 call_msg;
/* New Thread entry function */
void (* sub_callback)(const void *);
void subscription_callback(const void * msgin);
// Implementation example:

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
    rclc_support_t support;
    rclc_support_init(&support,0, NULL, &allocator);
    rcl_node_t node;
    rclc_executor_t executor;
    rcl_publisher_t publisher_critical_status;
    rcl_subscription_t subscriber_critical_status;

    rclc_node_init_default(&node, "faadihgas_node", "", &support);

    rclc_publisher_init_default(&publisher_critical_status,&node,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int8) , "/system_status/critical_as/state");
    rclc_subscription_init_default(&subscriber_critical_status, &node,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int8), "/system_status/critical_as/state");

    rcl_ret_t rc = rclc_executor_add_subscription(
      &executor, &subscriber_critical_status, &call_msg,
      &subscription_callback, ON_NEW_DATA);
    rclc_executor_spin(&executor);
    (void) rc;
    critical_as_int8<critical_as_state> temp_payload = {OFF};
    //critical_as_int8<critical_as_state> temp_payload_old = temp_payload;

    led_update(red, BSP_IO_LEVEL_LOW);
	while(1){
	    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);

	    //temp_payload = msg;
	    std_msgs__msg__Int8 payload = temp_payload.get();
	    auto publish_ret = rcl_publish(&publisher_critical_status, &payload, NULL);
	    (void) publish_ret;

	}

}
void subscription_callback(const void * msgin){
  // Cast received message to used type
    const std_msgs__msg__Int8 * msg_cast = (const std_msgs__msg__Int8 *)msgin;
    msg.data = msg_cast->data;

}

/*

	volatile UINT k=0;
	k=ros->error_counter;


    while (1){

    	auto publish_ret = rcl_publish(&publisher_critical_status, &msg, NULL);
    	(void) publish_ret;
    	if(temp_payload.state!=temp_payload_old.state ){
				tx_semaphore_get(&css, TX_DATA_HIGH_SPEED_TIMEOUT);
					store::critical_autonomous_system_status = temp_payload;
				tx_semaphore_put(&css);

			temp_payload_old = temp_payload;

		}

    	R_BSP_SoftwareDelay(10,BSP_DELAY_UNITS_MILLISECONDS);
	}
}


*/

