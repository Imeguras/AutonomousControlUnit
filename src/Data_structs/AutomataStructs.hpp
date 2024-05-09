/*
 * AutomataStructs.h
 *
 *  Created on: 09/12/2023
 *      Author: micron
 */
#include <stdint.h>
#include <std_msgs/msg/int8.h>
#pragma once
enum critical_as_state {
	OFF=1,
	READY=2,
	DRIVING=3,
	EMERGENCY=4,
	FINISH=5,
	UNKNOWN

};

template<typename T>
class critical_as_int8{
	public: 
		T state;
		critical_as_int8(){
			state = UNKNOWN;
		}
		critical_as_int8(T _state){
			this->state = _state;
			msg = *(std_msgs__msg__Int8__create());
			msg.data = _state;
		}
		std_msgs__msg__Int8 get(){
		    return msg;
		}
		void set(std_msgs__msg__Int8 _msg){
		    this->msg = _msg;

		}
		void set(T _state){
		    this->state=_state;
		}
		void operator=(T _state){
		    this->set(_state);
		}
		void operator=(std_msgs__msg__Int8 _msg){
		    this->set(_msg);
		}


	private: 
		std_msgs__msg__Int8 msg;

};



