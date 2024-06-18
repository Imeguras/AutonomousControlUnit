
/**
 * @file utils.h
 *
 *  @date Created on: 23/04/2024
 *  @author João Vieira
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hal_data.h"
#include <unordered_map>



#ifndef LART_ACUITY_UTILS_H_
#define LART_ACUITY_UTILS_H_
#include <stdint.h>


/**
 * Just a simple type to be more readable
 * OCCUPIED: describes the state when a can bus is already opened and fully operational
 * AVAILABLE: describes the state when a can bus can presumably open communication
 * UNAVAILABLE: describes the state when a can bus either was configured in the "kernel" to not be operational or external hardware issues exist
 * ERROR:  describes the state when a can bus is facing errors or problems.
 */
typedef enum{
    OCCUPIED=0, /**< OCCUPIED */
    AVAILABLE=1, /**< AVAILABLE */
    UNAVAILABLE=2, /**< UNAVAILABLE */
    ERROR       /**< ERROR */
}e_acuity_can_status;


typedef enum {
    CANFD_LITE_UNIT0_CHANNEL0 = 1,
    CANFD_LITE_UNIT0_CHANNEL1 = 2,
    CANFD_EXTERNAL_SPI_CHANNEL0 = 3
}e_total_can_channels;
/**
 * "legacy" getter for the state of a channel in the  can bus

 * @return e_acuity_can_status, an enum that describes the state of the can bus
 */
e_acuity_can_status g_canStateKernel(uint16_t id);
/**
 *
 * setter for the state of a channel in the  can bus
 * @param id_target the id of the virtualized channel to be changed
 * @param ingress_status the desired state of the can bus
 */
void s_canStateKernel(uint16_t id_target, e_acuity_can_status ingress_status);
/**
 *
 */
std::unordered_map<uint16_t, e_acuity_can_status>  g_canStateKernelMap();
/**
 * A map to keep track of the state of the can buses, its just an aid and shouldn't be used for non setup purposes
*/
static std::unordered_map<uint16_t, e_acuity_can_status> g_bsp_prv_can_t {
    {CANFD_LITE_UNIT0_CHANNEL0, AVAILABLE},
    {CANFD_LITE_UNIT0_CHANNEL1, AVAILABLE},
    {CANFD_EXTERNAL_SPI_CHANNEL0, AVAILABLE}
};

#endif
