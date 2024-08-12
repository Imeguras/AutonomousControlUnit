/*
 * HighSpeedAbsL.hpp
 *
 *  Created on: 07/10/2023
 *      Author: micron
 */
#include "AbstractPeripheralLayer.cpp"
#include "utils.h"
#include <iostream>
#include <memory>
#include <utility>
#include <iostream>
#include "../../../ra/board/ra8t1_acuity_bsp/board_leds.hpp"
#ifndef HIGHSPEEDABSL_H_
#define HIGHSPEEDABSL_H_

template<typename APL> class HighSpeed_AbsL {
public:
	HighSpeed_AbsL();
	virtual ~HighSpeed_AbsL();
	HighSpeed_AbsL(const HighSpeed_AbsL &&other);
	HighSpeed_AbsL& operator=(const HighSpeed_AbsL&& other);
	std::shared_ptr<APL> g_AplHandle();
	APL*  operator->();


private:
	std::shared_ptr<APL> apl_handle;


};


#endif /* HIGHSPEEDABSL_H_ */
