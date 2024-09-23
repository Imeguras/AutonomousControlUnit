/*
 * Store.h
 *
 *  Created on: 14/12/2023
 *      Author: micron
 */
#include <cstdint>
#include <lart_msgs/msg/as_status.h>
#pragma once

#define __LART_MAX_PRESSURE_BAR_HYDRAULIC_FRONT_AXIS__ 200
#define __LART_MIN_PRESSURE_BAR_HYDRAULIC_FRONT_AXIS__ 60
#define __LART_MAX_PRESSURE_BAR_HYDRAULIC_REAR_AXIS__ 200
#define __LART_MIN_PRESSURE_BAR_HYDRAULIC_REAR_AXIS__ 60

#define __LART_MIN_PRESSURE_BAR_PNEUMATIC_FRONT_AXIS__ 5.5f
#define __LART_MAX_PRESSURE_BAR_PNEUMATIC_FRONT_AXIS__ 10.0f
#define __LART_MIN_PRESSURE_BAR_PNEUMATIC_REAR_AXIS__ 5.5f
#define __LART_MAX_PRESSURE_BAR_PNEUMATIC_REAR_AXIS__ 10.0f

namespace store {

    typedef struct {
        int32_t target_position;
        int32_t target_speed;
        int32_t actual_position;
        int32_t actual_speed;
        int16_t actual_torque;
        bool undirty=true;
    }_Maxon_t;
    typedef struct {
        float pressureFront;
        float pressureRear;
    }_Pressure_Hydraulic_t;
    typedef struct {
        float pressureFront0;
        float pressureRear0;
        float pressureFront1;
        float pressureRear1;
        float transducer0;
        float transducer1;
    }_Pressure_Pneumatic_t;
    typedef struct{
        float voltage;
        float temperature;

    }_ACU_Internals_t;
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
                //TCU

                bool shutdown_circuit_state = false; //false = open, true = closed
                //INTERNAL
                _Maxon_t maxon;
                _GrossFunkeRes_t res;
                _ACU_Internals_t acu_internals;
                _Pressure_Hydraulic_t pressure_hydraulic;
                _Pressure_Pneumatic_t pressure_pneumatic;



    };
}

