/*
 * utils.cpp
 *
 *  Created on: 04/12/2023
 *      Author: micron
 */

#include "utils.h"

#include "../ra/board/ra8m1_acuity_bsp/board_init.hpp"


e_acuity_can_status g_canStateKernel(uint16_t id){
    return g_bsp_prv_can_t[id];
}

void s_canStateKernel(uint16_t id_target, e_acuity_can_status ingress_status){
    //check if id_target exists
    if(g_bsp_prv_can_t.find(id_target) != g_bsp_prv_can_t.end()){
        g_bsp_prv_can_t[id_target] = ingress_status;
    }

}

std::unordered_map<uint16_t, e_acuity_can_status>  g_canStateKernelMap(){
    //return a copy of the map
    //TODO: Check if its a copy
    //TODO: Welp now its most likely a reference...
    return g_bsp_prv_can_t;
}


#if (BSP_CFG_RTOS == 1)
/*
 * global variables
 */
TX_QUEUE g_rtt_op_data_queue;
TX_QUEUE g_rtt_ip_data_queue;
TX_EVENT_FLAGS_GROUP g_rtt_event;
TX_BYTE_POOL   g_byte_pool;

static CHAR *g_byte_pool_name = "rtt_processing_block";
static CHAR byte_memory[BYTE_POOL_SIZE] = {'\0'};
static uint8_t queue_memory_op_rtt_obj[20] = {'\0'};
static uint8_t queue_memory_ip_rtt_obj[20] = {'\0'};
#endif

UINT rtt_frameowrk_init(VOID)
{
#if (BSP_CFG_RTOS == 1)

    UINT err= TX_SUCCESS;

    err = tx_byte_pool_create(&g_byte_pool,g_byte_pool_name,(VOID *)&byte_memory[0],BYTE_POOL_SIZE);
    if (TX_SUCCESS != err)
    {
        return err;
    }

    err = tx_queue_create(&g_rtt_op_data_queue, (CHAR *)"rtt_op_queue", 1, &queue_memory_op_rtt_obj, sizeof(queue_memory_op_rtt_obj));
    if (TX_SUCCESS !=err)
    {
        tx_byte_pool_delete(&g_byte_pool);
        return err;
    }

    err = tx_queue_create(&g_rtt_ip_data_queue, (CHAR *)"dhcp_client_thread_input_queue", 1, &queue_memory_ip_rtt_obj, sizeof(queue_memory_ip_rtt_obj));
    if (TX_SUCCESS !=err)
    {
        tx_byte_pool_delete(&g_byte_pool);
        tx_queue_delete(&g_rtt_op_data_queue);
    }

    return err;
#endif
}

/*
 * Can be called any where after rtt_framework_init() function call
 * for graceful clean up
 */
VOID rtt_thread_service_cleanup(VOID)
{
#if (BSP_CFG_RTOS == 1)
    tx_byte_pool_delete(&g_byte_pool);

    /* if any user specific queue has been added, it should be clean up as per specific EP */
    tx_queue_delete(&g_rtt_op_data_queue);
    tx_queue_delete(&g_rtt_ip_data_queue);

    /* halt */
    APP_ERR_TRAP(true);
#endif
}
