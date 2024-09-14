/*
 * AdcRen.h
 *
 *  Created on: 14/09/2024
 *      Author: micron
 */
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#include "../../../../ra/board/board_ra8m1_acuity_bsp/board.h"
#include "../../../../ra/board/ra8m1_acuity_bsp/board_leds.hpp"
#include "../../../../ra/board/ra8m1_acuity_bsp/board_init.hpp"
#include "../../AbstractPeripheralLayer.cpp"
#include "r_adc.h"
#include "r_adc_api.h"

#ifndef INTERFACES_DRIVERS_HARDWAREBASED_ADCREN_H_
#define INTERFACES_DRIVERS_HARDWAREBASED_ADCREN_H_





extern const adc_instance_t g_adc0;

extern adc_instance_ctrl_t g_adc0_ctrl;
extern const adc_cfg_t g_adc0_cfg;
extern const adc_channel_cfg_t g_adc0_channel_cfg;

class AdcRen : AbstractPeripheralLayer{

    public:

        AdcRen();
        virtual ~AdcRen();
        int initialization() override;
        uint32_t recv(void * data, uint32_t stream_size) override;
        uint32_t write(void *data, uint32_t stream_size=0) override;
        uint32_t close();
    protected:
        volatile bool g_ready_to_read = false;

};

#endif /* INTERFACES_DRIVERS_HARDWAREBASED_ADCREN_H_ */
