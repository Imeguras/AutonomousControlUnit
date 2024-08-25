/*
 * Store.h
 *
 *  Created on: 14/12/2023
 *      Author: micron
 */
#include <cstdint>
#include <lart_msgs/msg/as_status.h>
#pragma once


namespace store {

    typedef struct {
        int32_t target_position;
        int32_t target_speed;
        int32_t actual_position;
        int32_t actual_speed;
        int16_t actual_torque;
        bool undirty=true;
    }_Maxon_t;
    typedef struct{
        uint8_t gpio_state;

        bool undirty=true;

    }_GrossFunkeRes_t;

    class Store {
        public:
            static Store& getInstance(){
                    static Store    instance;
                    return instance;
            }
            private:
                Store() {}
            public:
                Store(Store const&)           = delete;
                void operator=(Store const&)  = delete;
                //ROS2
                lart_msgs__msg__ASStatus as_status;

                //INTERNAL
                _Maxon_t maxon;
                _GrossFunkeRes_t res;




    };
}

