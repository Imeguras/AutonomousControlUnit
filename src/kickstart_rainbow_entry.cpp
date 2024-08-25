#include "kickstart_rainbow.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "../ra/board/ra8m1_acuity_bsp/board_leds.hpp"
#include "hal_data.h"

#define TIME_JAN 281
#define TIME_FEB 269
#define TIME_MAR 288
#define TIME_APR 291
#define TIME_MAY 295
#define TIME_JUN 301
#define TIME_JUL 299
#define TIME_AUG 285
#define TIME_SEP 296
#define TIME_OCT 294
#define TIME_NOV 307
#define TIME_DEC 268

#define __LART_RTC_24H___
#define __LART_RTC_ADJUST_IMEDIATE___

/* New Thread entry function */

void kickstart_rainbow_entry(void){

    rainbow();
    rtc_time_t time_f;
    R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg);
    #ifdef __LART_RTC_ADJUST_IMEDIATE___
        //        char time_str[10];
        //        strcpy(time_str, __TIME__);
        //        sscanf(time_str, "%d:%d:%d", &hour, &min, &sec);
        //        time_f.tm_hour = hour;
        //        time_f.tm_min = min;
        //        time_f.tm_sec = sec;
        //        int day, month, year;
        //        sscanf(__DATE__, "%d %d %d", &month, &day, &year);
        //        time_f.tm_mday = day;
        //        time_f.tm_mon = month;
        //        time_f.tm_year = year - 1900;


        //Welcome to the axanato
        time_f.tm_hour = (__TIME__[0] - '0')*10 + __TIME__[1] - '0';
        time_f.tm_min = (__TIME__[3] - '0')*10 + __TIME__[4] - '0';
        time_f.tm_sec = (__TIME__[6] - '0')*10 + __TIME__[7] - '0';

        //for the month is Mmm dd yyyy
        //Welcome to further axanato, someone should get fired for this blunder
        int sum =  __DATE__[0]+__DATE__[1]+__DATE__[2];
        switch(sum){
        case TIME_JAN:
            time_f.tm_mon = 1;
            break;
        case TIME_FEB:
            time_f.tm_mon = 2;
            break;
        case TIME_MAR:
            time_f.tm_mon = 3;
        break;
        case TIME_APR:
            time_f.tm_mon = 4;
        break;
        case TIME_MAY:
            time_f.tm_mon = 5;
            break;
        case TIME_JUN:
            time_f.tm_mon = 6;
            break;
        case TIME_JUL:

            time_f.tm_mon = 7;
        break;
        case TIME_AUG:
            time_f.tm_mon = 8;
        break;
        case TIME_SEP:
            time_f.tm_mon = 9;
        break;
        case TIME_OCT:
            time_f.tm_mon = 10;
        break;
        case TIME_NOV:
            time_f.tm_mon = 11;
        break;
        case TIME_DEC:
            time_f.tm_mon = 12;
        break;
        default:
            time_f.tm_mon = 0;
        break;
        }
        time_f.tm_mday = (__DATE__[4] - '0')*10 + __DATE__[5] - '0';
        time_f.tm_year = ((__DATE__[7] - '0')*1000 + (__DATE__[8] - '0')*100 + (__DATE__[9] - '0')*10 + (__DATE__[10] - '0')) - 1900;
        R_RTC_CalendarTimeSet(&g_rtc0_ctrl, &time_f);
        #ifdef __LART_RTC_24H___
            R_RTC->RCR2_b.HR24 = 1;

        #endif
#endif




    R_RTC_CalendarTimeGet(&g_rtc0_ctrl, &time_f);
    if(time_f.tm_hour<12){
        leds_update(red, BSP_IO_LEVEL_HIGH);
        leds_update(blue, BSP_IO_LEVEL_LOW);
     }

}
