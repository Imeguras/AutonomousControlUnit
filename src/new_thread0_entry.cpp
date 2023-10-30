#include "HighSpeedAbsL.cpp"
#include "new_thread0.h"
#include "EthDuo.h"
#include "CanRen.h"
#include <string.h>
#include <functional>
HighSpeed_AbsL<CanRen> * can_ptr = nullptr;

extern bsp_leds_t g_bsp_leds;
static can_frame_t temp;
static volatile bool t;
/* New Thread entry function */
void new_thread0_entry(void) {
	//
	//HighSpeed_AbsL<EthDuo> ethernet;
	HighSpeed_AbsL<CanRen> can;
	can_ptr=&can;

	st_can_frame blah;
	blah.id = 0x41;
	uint64_t data= 0x1100220033004400;

	blah.type = CAN_FRAME_TYPE_DATA;
	blah.data_length_code = 8;
	blah.id_mode =CAN_ID_MODE_STANDARD;
	memcpy(blah.data,(void *) &data, sizeof(data));

	volatile UINT k=0;
	t=false;

	blah = temp;
	k=0;
	//k = can->write((void *)&blah, blah.data_length_code);
	//R_BSP_SoftwareDelay(10,BSP_DELAY_UNITS_SECONDS);
	//R_BSP_SoftwareDelay(2, BSP_DELAY_UNITS_SECONDS);
	//blah=*((st_can_frame *)can->recv(&blah,1));
	if(blah.id){
		blah.id=0x77;
		//printf("Welp: %s", blah.data );
		//k = can->write((void *)&blah, blah.data_length_code);

	}



	//R_BSP_SoftwareDelay(10,BSP_DELAY_UNITS_SECONDS);
			//can.write((void *)&blah, 0);
		//	acu.g_AplHandle();
	 //can->write((void *)&blah, blah.data_length_code);
    //k +=1;
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
/* Callback function */
void can_callback(can_callback_args_t *p_args){
	/**if (can_ptr != nullptr){
		(*can_ptr)->can_callback(p_args);
	}**/
    switch (p_args->event) {


        case CAN_EVENT_TX_COMPLETE:


            break;


        case CAN_EVENT_RX_COMPLETE:
            //b_can_rx = true;
        	//rx_ready=true;


        	t=true;
            memmove(&temp, &p_args->frame, sizeof(can_frame_t));  //copy the received data to rx_frame
           // this->buffered_mails_rx.push_back(temp);
            break;

        case CAN_EVENT_MAILBOX_MESSAGE_LOST:    //overwrite/overrun error event
        case CAN_EVENT_BUS_RECOVERY:            //Bus recovery error event
        case CAN_EVENT_ERR_BUS_OFF:             //error Bus Off event
        case CAN_EVENT_ERR_PASSIVE:             //error passive event
        case CAN_EVENT_ERR_WARNING:             //error warning event
        case CAN_EVENT_ERR_BUS_LOCK:            //error bus lock
        case CAN_EVENT_ERR_CHANNEL:             //error channel
        case CAN_EVENT_ERR_GLOBAL:              //error global
        case CAN_EVENT_TX_ABORTED:              //error transmit abort
        case CAN_EVENT_TX_FIFO_EMPTY:           //error transmit FIFO is empty
        case CAN_EVENT_FIFO_MESSAGE_LOST:       //error FIFO message lost
        	//TODO actually do something

            //b_can_err = true;                   //set flag bit
            break;
    }

}
