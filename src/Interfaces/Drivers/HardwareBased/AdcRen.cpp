/*
 * AdcRen.cpp
 *
 *  Created on: 14/09/2024
 *      Author: micron
 */

#include "AdcRen.h"

AdcRen::AdcRen()
{

}

AdcRen::~AdcRen()
{
}

int AdcRen::initialization(){
    fsp_err_t err = FSP_SUCCESS;


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
uint32_t AdcRen::close(){
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
/**
 * @brief Read data from the ADC peripheral.
 * @param data pointer to the buffer where the data will be stored, it must be allocated for at least 2 bytes and the first byte entails the channel
 * @param stream_size size of the alocated buffer in bytes
 * @return 0 if the operation was successful, otherwise returns the error code (see fsp_err_t in fsp_common.h)
 */
uint32_t AdcRen::recv(void * data, uint32_t stream_size){
    fsp_err_t err = FSP_SUCCESS;     // Error status
    if(stream_size ==0){
        return FSP_ERR_INVALID_SIZE;

    }
    //check if data is allocated
    if (data == NULL)
    {
        return FSP_ERR_INVALID_POINTER;
    }
    int8_t _channel = *(int8_t *)data;
    adc_channel_t channel = (adc_channel_t)_channel;
    uint16_t g_adc_data = 0;
    /* Read the result */
    err = R_ADC_Read (&g_adc0_ctrl, channel, &g_adc_data);
    /* handle error */
    if (FSP_SUCCESS != err)
    {
        /* ADC Failure message */
        return err;
    }




    /* In adc single scan mode after reading the data, it stops.So reset the g_ready_to_read state to
     * avoid reading unnecessarily. close the adc module as it gets opened in start scan command.*/
    if (ADC_MODE_SINGLE_SCAN == g_adc0_cfg.mode){
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
    return err;

}
/**
 * @brief Write data to the peripheral, since this method is related to an ADC it will not do anything.
 * @param data
 * @param stream_size
 * @return always 0
 */
uint32_t AdcRen::write(void *data, uint32_t stream_size){
    FSP_PARAMETER_NOT_USED(data);
    FSP_PARAMETER_NOT_USED(stream_size);

    return 0;
}
