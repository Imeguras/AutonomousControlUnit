#include "supervisor_thread.h"
#include "../ra/board/ra8m1_acuity_bsp/board_leds.hpp"
#include "utils.h"
#include "Interfaces/Drivers/HardwareBased/AdcRen.h"
#include "Interfaces/HighSpeedAbsL.cpp"
/* Supervisor_Thread entry function */
void refresh ();
bsp_io_level_t WhatchdogFlip = BSP_IO_LEVEL_LOW;
/*******************************************************************************************************************//**
 * @addtogroup adc_ep
 * @{
 **********************************************************************************************************************/

/* local variables */


/*
 * private function declarations
 */
/* Open the adc module, configures and initiates the scan*/



void supervisor_thread_entry(void){
    HighSpeed_AbsL<AdcRen> unit0 ;

    while (1){
        refresh();

        R_BSP_SoftwareDelay(100,BSP_DELAY_UNITS_MILLISECONDS);
    }
}
void refresh (){
    if (WhatchdogFlip == BSP_IO_LEVEL_LOW){
        WhatchdogFlip = BSP_IO_LEVEL_HIGH;
    }else{
        WhatchdogFlip = BSP_IO_LEVEL_LOW;
    }
      R_BSP_PinAccessEnable();
      R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) 0x0B03, WhatchdogFlip);
      R_BSP_PinAccessDisable();

}









/*******************************************************************************************************************//**
 * @} (end addtogroup adc_ep)
 **********************************************************************************************************************/


