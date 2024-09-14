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
    //allocate 4 bytes
    uint8_t * mem= NULL;
    mem=(uint8_t *)malloc(4*sizeof(uint8_t));
    //check if allocation failed
    if(mem == NULL){
        return;
    }
    mem[0] = ADC_CHANNEL_TEMPERATURE;
    while (1){
        unit0->recv(mem,4);
        uint16_t adc_data = 0;
        //retrieve adc_data from mem[1] and mem[2]
        memcpy(&adc_data, mem+1, sizeof(uint16_t));

        float temperature = convert_adc_data_temperature(adc_data);

        if(temperature < 100.0f || temperature > -30.0f){
            refresh();
        }

        R_BSP_SoftwareDelay(100,BSP_DELAY_UNITS_MILLISECONDS);
    }

    free(mem);
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


