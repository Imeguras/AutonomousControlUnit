/*
 * CANopenStack_test.cpp
 *
 *  Created on: 20/06/2024
 *      Author: micron
 */

#include "../Interfaces/Drivers/CANopenStack.h"
#include <unity.h>
#include "TestTemplate.hpp"
class CANopenStack_test : TestTemplate  {
public:
    CANopenStack_test(){
        setUp();
    }
    void setUp() override{

    }
    void runAllTests() override{
        RUN_TEST(spec_CANopenStack_bindMechanism);
    }

    static void spec_CANopenStack_bindMechanism(){
        TEST_ASSERT_EQUAL_INT(0,0);
    }

};


