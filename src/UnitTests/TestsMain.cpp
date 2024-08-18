/*
 * TestsMain.cpp
 *
 *  Created on: 17/08/2024
 *      Author: micron
 */
#include <unity.h>
#include <vector>
#include "CANopenStack_test.cpp"
class TestsMain{
public:

    static void runAllTests(){
        UNITY_BEGIN();
        CANopenStack_test k;
        k.runAllTests();
        UNITY_END();
    }

};

