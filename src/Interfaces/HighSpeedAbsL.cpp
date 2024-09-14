/*
 * HighSpeedAbsL.cpp
 *
 *  Created on: 07/10/2023
 *      Author: micron
 */

#include "HighSpeedAbsL.h"

#include "../../../ra/board/ra8m1_acuity_bsp/board_leds.hpp"

template<typename APL>
APL*  HighSpeed_AbsL<APL>::operator->(){
	APL* weak =this->apl_handle.get();
	//TODO: if theres no lock there might be dangling memory pointers.
	return weak;
}
//TODO Here would be the place to destroy a shared pointers with an operator overload IF I HAD ONE



template<typename APL>
HighSpeed_AbsL<APL>::HighSpeed_AbsL() {
    try{
        std::shared_ptr<APL> _temp = std::make_shared<APL>();
        std::weak_ptr<APL> weak = _temp;
            // Access the object using weak_ptr
            // Lock it
            if (auto locked = weak.lock()){

                int init_ret = locked->initialization();
                //Init is OK?
                if(FSP_SUCCESS == (fsp_err_t)init_ret){
                    this->apl_handle = locked;
                }else{
                    //TODO define a screaming protocol
                }

        } else {
            //TODO define a better screaming protocol
            //led_update(red, BSP_IO_LEVEL_HIGH);
        }
    }catch(...){
//        led_blink(0,5);
//        return;
    }

}

template<typename APL> HighSpeed_AbsL<APL>::~HighSpeed_AbsL() {
	this->apl_handle.reset();

}

template<typename APL> HighSpeed_AbsL<APL>::HighSpeed_AbsL(const HighSpeed_AbsL&& other) {
    // Check for self-assignment to avoid unnecessary work
    if (this == &other)
    {
        return;
    }

    this->apl_handle = std::move (other.apl_handle);
    other.~HighSpeed_AbsL ();
    return;
}
template<typename APL>
std::shared_ptr<APL> HighSpeed_AbsL<APL>::g_AplHandle(){
	//TODO: yeah this is dangerous af
	//std::weak_ptr<APL> weak = apl_handle;


    return std::make_shared<APL>(*apl_handle);
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





