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
		critical_as_int8(T state){
			this->state = state;
			msg = *(std_msgs__msg__Int8__create());
			msg.data = state;	
		}
		std_msgs__msg__Int8 get(){
		    return msg;
		}
		void set(std_msgs__msg__Int8 msg){
		    this->msg = msg;

		}
		void set(T state){
		    this->state=state;
		}
		void operator=(T state){
		    this->set(state);
		}
		void operator=(std_msgs__msg__Int8 msg){
		    this->set(msg);
		}


	private: 
		std_msgs__msg__Int8 msg;

};



