/*
 * utils.cpp
 *
 *  Created on: 04/12/2023
 *      Author: micron
 */

#include "utils.h"

#include "board_init.hpp"


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
