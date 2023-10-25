#include "HighSpeedAbsL.cpp"
#include "new_thread0.h"
#include "EthDuo.h"
#include "CanRen.h"
#include <string.h>
//#include "CanRen.h"
extern bsp_leds_t g_bsp_leds;

/* New Thread entry function */
void new_thread0_entry(void) {
	//
	//HighSpeed_AbsL<EthDuo> ethernet;
	//auto k = ethernet.g_AplHandle();
	HighSpeed_AbsL<CanRen> can;
	//auto f = can.g_AplHandle();


	//CanRen can;
	//CanRen canRen;

	st_can_frame blah;
	blah.id = 0x41;

	blah.id_mode =CAN_ID_MODE_STANDARD;
	blah.type = CAN_FRAME_TYPE_DATA;
	blah.data_length_code = 8;
	blah.data[0]=0xAA;
	blah.data[1]=0x11;
	blah.data[2]=0x11;
	blah.data[3]=0x11;
	blah.data[4]=0x11;
	blah.data[5]=0x11;
	blah.data[6]=0x11;
	blah.data[7]=0x11;

	can->write((void *)&blah, blah.data_length_code);
	R_BSP_SoftwareDelay(10,BSP_DELAY_UNITS_SECONDS);
			//can.write((void *)&blah, 0);
		//	acu.g_AplHandle();
	UINT k = can->write((void *)&blah, blah.data_length_code);
    k +=1;
	//R_BSP_SoftwareDelay(10,BSP_DELAY_UNITS_SECONDS);
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

    R_BSP_PinAccessEnable();

          /* Update all board LEDs */


              //uint32_t pin = leds.p_leds[count];
          	//R_SCI_UART_Write(&g_uart0_ctrl, (const uint8_t *)"blah",5);
              //
              if(k<=0){
              	R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[0], BSP_IO_LEVEL_HIGH);
              }else{
              	R_BSP_PinWrite((bsp_io_port_pin_t)leds.p_leds[2], BSP_IO_LEVEL_HIGH);
              }



          /* Protect PFS registers */
          R_BSP_PinAccessDisable();
    while (1)
    {
        /* Enable access to the PFS registers. If using r_ioport module then register protection is automatically
         * handled. This code uses BSP IO functions to show how it is used.
         */


        /* Toggle level for next write */

        tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND);
    }
}
