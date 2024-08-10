#include "kickstart_rainbow.h"


#include "../ra/board/ra8t1_acuity_bsp/board_leds.hpp"
/* KickStart_Rainbow entry function */
void kickstart_rainbow_entry(void){
    fsp_err_t err = FSP_SUCCESS;     // Error statuw}
    rainbow();
    //start High_Speed_interface thread


}

