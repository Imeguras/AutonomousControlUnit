/*
 * HighSpeedAbsL.cpp
 *
 *  Created on: 07/10/2023
 *      Author: micron
 */
#include "Drivers/HardwareBased/EthDuo.h"
#include "HighSpeedAbsL.h"

#include "../../../ra/board/ra8t1_acuity_bsp/board_leds.hpp"

template<typename APL>
APL*  HighSpeed_AbsL<APL>::operator->(){
	APL* weak =this->apl_handle.get();
	//TODO: if theres no lock there might be dangling memory pointers.
	return weak;
}
//TODO Here would be the place to destroy a shared pointers with an operator overload IF I HAD ONE



template<typename APL>
HighSpeed_AbsL<APL>::HighSpeed_AbsL() {
    //TODO : maybe we should first force initialization and only after everything goes according to plan the apl_handle is filled
	this->apl_handle = std::make_shared<APL>();
	std::weak_ptr<APL> weak = this->apl_handle;
	    // Access the object using weak_ptr
	    if (auto locked = weak.lock()) {
            locked->initialization();

	    } else {
	    	//TODO define a better screaming protocol
	        //led_update(red, BSP_IO_LEVEL_HIGH);

	        //std::cout << "Object has been destroyed." << std::endl;
	    }
}

template<typename APL> HighSpeed_AbsL<APL>::~HighSpeed_AbsL() {
	this->apl_handle.reset();

}

template<typename APL> HighSpeed_AbsL<APL>::HighSpeed_AbsL(const HighSpeed_AbsL&& other) {
	//TODO
}
template<typename APL>
std::weak_ptr<APL> HighSpeed_AbsL<APL>::g_AplHandle(){
	//TODO: yeah this is dangerous af
	std::weak_ptr<APL> weak = apl_handle;
	return weak;
}
template<typename APL>
HighSpeed_AbsL<APL>& HighSpeed_AbsL<APL>::operator=(const HighSpeed_AbsL<APL>&& other) {
	// Check for self-assignment to avoid unnecessary work
			if (this == &other) {
				return *this;
			}

			this->apl_handle= std::move(other.apl_handle);
			other.~HighSpeed_AbsL();
			return *this;
}





