/*
 * utils.cpp
 *
 *  Created on: 04/12/2023
 *      Author: micron
 */

#include "utils.h"
extern bsp_leds_t g_bsp_leds;

 void led_update(led_state_t led_state, e_bsp_io_level value){
	//BSP_IO_LEVEL_LOW
	//BSP_IO_LEVEL_HIGH
   R_BSP_PinAccessEnable();
    switch(led_state){
        case red:
        {

            R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) g_bsp_leds.p_leds[2], value);
            break;
        }
        case green:
        {

            R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) g_bsp_leds.p_leds[1], value);

            break;
        }
        case blue:
        {
            /* Blue LED state is made high to show operation is in progress */
            R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) g_bsp_leds.p_leds[0], value);
            break;
        }
        default:
        {
            break;
        }
    }
    R_BSP_PinAccessDisable();
}
