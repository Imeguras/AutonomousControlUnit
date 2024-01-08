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
enum critical_as_mission {
    ACCELERATION=1,
    SKIDPAD=2,
    TRACKDRIVE=3,
    BRAKEDRIVE=4,
    INSPECTION=5,
    AUTOCROSS=6,
    UNKNOWN
};

template<typename T>
class wrapper_int8{
	public: 
		T state;

		wrapper_int8(){
			state = UNKNOWN;
		}
		wrapper_int8(T incoming_state){

			this->state = incoming_state;
			msg = *(std_msgs__msg__Int8__create());
			msg.data = incoming_state;
		}
		std_msgs__msg__Int8 get(){
		    return msg;
		}
		void set(std_msgs__msg__Int8 incoming_msg){
		    this->msg = incoming_msg;
		    this->state= (critical_as_state)incoming_msg.data;


		}
		void set(T incoming_state){
		    this->state=incoming_state;
		    this->msg= *(std_msgs__msg__Int8__create());
		    msg.data = (int8_t)incoming_state;
		}
		void operator=(T incoming_state){
		    this->set(incoming_state);
		}
		void operator=(std_msgs__msg__Int8 incoming_msg){
		    this->set(incoming_msg);
		}
		bool operator!=(wrapper_int8<T> other){
		    return this->state != other.state;
		}


	private: 
		std_msgs__msg__Int8 msg;

};



