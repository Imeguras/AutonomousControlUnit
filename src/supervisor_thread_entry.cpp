#include "supervisor_thread.h"
#include "../ra/board/ra8m1_acuity_bsp/board_leds.hpp"
#include "utils.h"
#include "Interfaces/Drivers/HardwareBased/AdcRen.h"
#include "Interfaces/HighSpeedAbsL.cpp"
#include "Data_structs/Store.cpp"

#define __LART_SECURITY_AS_BYPASS__
#define __LART_SECURITY_AS_BYPASS__EXTERNAL_TCU__

void refresh ();
void emergency();
uint32_t initial_checkup_sequence();
bsp_io_level_t WhatchdogFlip = BSP_IO_LEVEL_LOW;





void supervisor_thread_entry(void){

    HighSpeed_AbsL<AdcRen> unit0 ;
    //allocate 4 bytes
    uint8_t * mem= NULL;
    mem=(uint8_t *)malloc(4*sizeof(uint8_t));
    //check if allocation failed
    if(mem == NULL){
        return;
    }

    while (1){
        /// Check shutdown circuit
        if (store::Store::getInstance ().shutdown_circuit_state == true){
            break;
        }
        /// CHECK IF CAN BUS IS ALIVE
        //TODO: this must be done...
//        if (g_canStateKernel (0) == e_acuity_can_status::CAN_STATUS_OFFLINE)
//        {
//            emergency ();
//        }
        /// CHECK STATUS OF AS
        if (store::Store::getInstance ().as_status.state.data == lart_msgs__msg__State__EMERGENCY){
            emergency ();
        }
        /// CHECK STATUS OF RES
        if (store::Store::getInstance ().res.gpio_state == 0){
            emergency ();
        }
        /// CHECK STATUS OF MAXON
        //TODO: maxon stuff
        ///CHECK STATUS OF ACU
        //TODO: move boylerplate elsewhere
        mem[0] = ADC_CHANNEL_TEMPERATURE;
        unit0->recv(mem,4);
        uint16_t adc_data = 0;
        //retrieve adc_data from mem[1] and mem[2]
        memcpy(&adc_data, mem+1, sizeof(uint16_t));

        float temperature = convert_adc_data_temperature(adc_data);

        store::Store::getInstance ().acu_internals.temperature = temperature;

        if(temperature > 150.0f || temperature < -30.0f){
            emergency ();
        }
        mem[0] = ADC_CHANNEL_1;
        unit0->recv(mem,4);
        adc_data = 0;
        //retrieve adc_data from mem[1] and mem[2]
        memcpy(&adc_data, mem+1, sizeof(uint16_t));
        float pressure_front = convert_adc_data_pressure_(adc_data);
        store::Store::getInstance ().pressure_pneumatic.pressureFront0 = pressure_front;
        mem[0] = ADC_CHANNEL_3;
        unit0->recv(mem,4);
        adc_data = 0;
        //retrieve adc_data from mem[1] and mem[2]
        memcpy(&adc_data, mem+1, sizeof(uint16_t));
        float pressure_rear = convert_adc_data_pressure_(adc_data);
        store::Store::getInstance ().pressure_pneumatic.pressureFront0 = pressure_rear;


        ///CHECK STATUS OF PRESSURE
        if (store::Store::getInstance ().pressure_pneumatic.pressureFront0 < __LART_MIN_PRESSURE_BAR_PNEUMATIC_FRONT_AXIS__
                || store::Store::getInstance ().pressure_pneumatic.pressureFront0 > __LART_MAX_PRESSURE_BAR_PNEUMATIC_FRONT_AXIS__){
            emergency ();
        }
        if (store::Store::getInstance ().pressure_pneumatic.pressureRear0 < __LART_MIN_PRESSURE_BAR_PNEUMATIC_REAR_AXIS__
                || store::Store::getInstance ().pressure_pneumatic.pressureRear0 > __LART_MAX_PRESSURE_BAR_PNEUMATIC_REAR_AXIS__)
        {
            emergency ();
        }
        refresh();
        R_BSP_SoftwareDelay(100,BSP_DELAY_UNITS_MILLISECONDS);
    }
    /// BREAKOUT POINT

    /// DELAY 200MS
    R_BSP_SoftwareDelay(200,BSP_DELAY_UNITS_MILLISECONDS);
    ///CHECK IF FRONT AXIS
    if (store::Store::getInstance ().pressure_hydraulic.pressureFront < __LART_MIN_PRESSURE_BAR_HYDRAULIC_FRONT_AXIS__
            || store::Store::getInstance ().pressure_hydraulic.pressureFront > __LART_MAX_PRESSURE_BAR_HYDRAULIC_FRONT_AXIS__){
        emergency();
    }
    /// CHECK IF REAR AXIS
    if (store::Store::getInstance ().pressure_hydraulic.pressureRear < __LART_MIN_PRESSURE_BAR_HYDRAULIC_REAR_AXIS__
            || store::Store::getInstance ().pressure_hydraulic.pressureRear > __LART_MAX_PRESSURE_BAR_HYDRAULIC_REAR_AXIS__){
        emergency ();
    }
    while(1){
        tx_thread_sleep(1);
    }

    free(mem);

}
void emergency(){
    led_flip(7);
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


uint32_t initial_checkup_sequence(){
    while(1){
        #ifdef __LART_SECURITY_AS_BYPASS__EXTERNAL_TCU__
                store::Store::getInstance().shutdown_circuit_state = true;
        #endif
        if (store::Store::getInstance().shutdown_circuit_state == true){
            break;
        }
        refresh();
        R_BSP_SoftwareDelay(100,BSP_DELAY_UNITS_MILLISECONDS);
    }
    return FSP_SUCCESS;

}






/*******************************************************************************************************************//**
 * @} (end addtogroup adc_ep)
 **********************************************************************************************************************/


