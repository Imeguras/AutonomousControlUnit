#include "supervisor_thread.h"
#include "../ra/board/ra8m1_acuity_bsp/board_leds.hpp"
#include "utils.h"
/* Supervisor_Thread entry function */
void refresh ();
bsp_io_level_t WhatchdogFlip = BSP_IO_LEVEL_LOW;
/*******************************************************************************************************************//**
 * @addtogroup adc_ep
 * @{
 **********************************************************************************************************************/

/* local variables */
volatile bool g_ready_to_read = false;
static uint16_t g_adc_data;
static bool g_window_comp_event = false;

float g_adc_volt = {0x00};



/*
 * private function declarations
 */
/* Open the adc module, configures and initiates the scan*/
static fsp_err_t adc_scan_start(void);

/* stops the adc scan if the adc is continuous scan and then close the module */
static fsp_err_t adc_scan_stop(void);
static fsp_err_t adc_read_data(void);
void adc_close(void);
/* Callback to handle window compare event */
void adc_callback(adc_callback_args_t * p_args);
void supervisor_thread_entry(void){

    while (1){
        refresh();
        adc_scan_start();
        adc_read_data();
        if (g_adc_volt != 0){
            leds_update(red, BSP_IO_LEVEL_HIGH);
        }
        R_BSP_SoftwareDelay(100,BSP_DELAY_UNITS_MILLISECONDS);
    }
}
void refresh (){
    if (WhatchdogFlip == BSP_IO_LEVEL_LOW)
    {
        WhatchdogFlip = BSP_IO_LEVEL_HIGH;
    }
    else
    {
        WhatchdogFlip = BSP_IO_LEVEL_LOW;
    }
      R_BSP_PinAccessEnable();
      R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) 0x0B03, WhatchdogFlip);
      R_BSP_PinAccessDisable();

}
static fsp_err_t adc_scan_start(void){
    fsp_err_t err = FSP_SUCCESS;
    g_window_comp_event = false;

    if (false == g_ready_to_read){
        /* Open/Initialize ADC module */
        err = R_ADC_Open (&g_adc0_ctrl, &g_adc0_cfg);
        /* handle error */
        if (FSP_SUCCESS != err){

            return err;
        }

        /* Configures the ADC scan parameters */
        err = R_ADC_ScanCfg (&g_adc0_ctrl, &g_adc0_channel_cfg);
        /* handle error */
        if (FSP_SUCCESS != err){
            /* ADC Failure message */
            return err;
        }

        /* Start the ADC scan*/
        err = R_ADC_ScanStart (&g_adc0_ctrl);
        /* handle error */
        if (FSP_SUCCESS != err){
            /* ADC Failure message */
            return err;
        }

        /* Disable interrupts */
        R_BSP_IrqDisable((IRQn_Type)ADC_EVENT_SCAN_COMPLETE);
        /* Indication to start reading the adc data */
        g_ready_to_read = true;
    }else{
        return FSP_ERR_ALREADY_OPEN;
    }

    return err;
}
static fsp_err_t adc_scan_stop(void)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status

    /* Stop the scan if adc scan is started in continuous scan mode else ignore */
    if((ADC_MODE_SINGLE_SCAN != g_adc0_cfg.mode) && (true == g_ready_to_read ))
    {
        /* Stop ADC scan */
        err = R_ADC_ScanStop (&g_adc0_ctrl);
        /* handle error */
        if (FSP_SUCCESS != err)
        {
            /* ADC Failure message */
            return err;
        }
        /* reset to indicate stop reading the adc data */
        g_ready_to_read = false;

        /* Close the ADC module*/
        err = R_ADC_Close (&g_adc0_ctrl);
        /* handle error */
        if (FSP_SUCCESS != err){
            /* ADC Failure message */
            return err;
        }
    }

    return err;
}




/*******************************************************************************************************************//**
 * @brief    This function reads the adc output data from the prescribed channel and checks adc status
 * @param[IN]   None
 * @retval FSP_SUCCESS                  Upon successful stops the adc scan and closes the adc
 * @retval Any Other Error code apart from FSP_SUCCES  Unsuccessful stop or close of adc
 ***********************************************************************************************************************/
fsp_err_t adc_read_data(void){
    fsp_err_t err = FSP_SUCCESS;     // Error status

    /* Read the result */
    err = R_ADC_Read (&g_adc0_ctrl, ADC_CHANNEL_TEMPERATURE, &g_adc_data);
    /* handle error */
    if (FSP_SUCCESS != err)
    {
        /* ADC Failure message */
        return err;
    }


    //g_adc_volt = (float)((g_adc_data * 3.3f)/ADC_12_BIT);
    auto cal_127 = R_TSN_CAL->TSCDR;

    float v1 = 3.3f * (float)cal_127 / 4096.0f;
    float vs = 3.3f * (float)g_adc_data / 4096.0f;

    g_adc_volt = ((vs - v1) / (4.0f/1000)) + 127.0f;


    /* In adc single scan mode after reading the data, it stops.So reset the g_ready_to_read state to
     * avoid reading unnecessarily. close the adc module as it gets opened in start scan command.*/
    if (ADC_MODE_SINGLE_SCAN == g_adc0_cfg.mode || g_window_comp_event == true){
        g_ready_to_read = false;

        /* Stop ADC scan */
        err = R_ADC_ScanStop (&g_adc0_ctrl);
        /* Handle error */
        if (FSP_SUCCESS != err)
        {   /* ADC ScanStop message */
            return err;
        }



        /* Close the ADC module*/
        err = R_ADC_Close (&g_adc0_ctrl);
        /* handle error */
        if (FSP_SUCCESS != err)
        {
            /* ADC Failure message */
            return err;
        }

    }


    /* 1 Seconds Wait time between successive readings */
    R_BSP_SoftwareDelay (1, BSP_DELAY_UNITS_SECONDS);
    return err;
}




/*******************************************************************************************************************//**
 * @brief    Close the adc driver and Handle the return closing API error, to the Application.
 * @param[IN]   None
 * @retval None.
 ***********************************************************************************************************************/
void adc_close(void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* close the ADC driver */
    err = R_ADC_Close (&g_adc0_ctrl);

    /* handle error */
    if (FSP_SUCCESS != err)
    {
        /* GPT Close failure message */
        APP_ERR_PRINT("** R_ADC_Close API failed **  \r\n");
    }
}




/* Callback procedure for when window A compare event occurs */
void adc_callback(adc_callback_args_t * p_args)
{
    if(ADC_EVENT_WINDOW_COMPARE_A == p_args->event)
    {
        g_window_comp_event = true;
        IRQn_Type irq = R_FSP_CurrentIrqGet();
        R_BSP_IrqDisable(irq);
    }

}

/*******************************************************************************************************************//**
 * @} (end addtogroup adc_ep)
 **********************************************************************************************************************/


