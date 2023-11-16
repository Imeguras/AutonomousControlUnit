#include "rmw_microros/rmw_microros.h"
#include "MicroRosHumble/microros_transports.h"

#include <high_speed_interface_thread0.h>
#include "HighSpeedAbsL.cpp"
#include "MicroRosHumble/EthDuo.h"
#include "MicroRosHumble/MicroRosDuo.h"
#include <string.h>
#include <functional>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int32.h>
extern bsp_leds_t g_bsp_leds;

static volatile bool t;
/* New Thread entry function */
void high_speed_interface_thread0_entry(void) {

	HighSpeed_AbsL<EthDuo> ros;
	auto k = ros->initialization();

/*
	volatile UINT k=0;
	k=ros->error_counter;

	rcl_allocator_t allocator = rcl_get_default_allocator();
	rclc_support_t support;
	rclc_support_init(&support,0, NULL, &allocator);
	rcl_node_t node;
	rclc_node_init_default(&node, "faadihgas_node", "", &support);
	rcl_publisher_t publisher;
	rclc_publisher_init_default(&publisher,&node,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32) , "/faadihgas");
	std_msgs__msg__Int32 msg;
	/* LED type structure */
	bsp_leds_t leds = g_bsp_leds;
    /* If this board has no LEDs then trap here */
	if (0 == leds.led_count)
    {
        while (1)
        {
            ;                          // There are no LEDs on this board
        }
    }
	//fsp_err_t err = FSP_SUCCESS;

	//err= R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);



    /* Holds level to set for pins */
	bsp_io_level_t pin_level = BSP_IO_LEVEL_LOW;

	R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_SECONDS);
    R_BSP_PinAccessEnable();

          /* Update all board LEDs */

              if(k<=0){
              	R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[0], BSP_IO_LEVEL_HIGH);
              }else{
              	R_BSP_PinWrite((bsp_io_port_pin_t)leds.p_leds[2], BSP_IO_LEVEL_HIGH);
              }




	R_BSP_PinAccessDisable();
    while (1){
        /* Enable access to the PFS registers. If using r_ioport module then register protection is automatically
         * handled. This code uses BSP IO functions to show how it is used.
         */
    	//rcl_publish(&publisher, &msg, NULL); // @suppress("Return value not evaluated")
    	//msg.data++;
    	R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_SECONDS);
		}

}
