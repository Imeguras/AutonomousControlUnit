/*
 * TestTemplate.hpp
 *
 *  Created on: 18/08/2024
 *      Author: micron
 */
#pragma once

class TestTemplate{
    public:
        TestTemplate(){

        }
        virtual void setUp() = 0;
        virtual void runAllTests() = 0;
        virtual ~TestTemplate() = 0;
};

