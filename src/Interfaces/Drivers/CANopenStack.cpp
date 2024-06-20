/*
 * CANopenStack.cpp
 *
 *  Created on: 20/06/2024
 *      Author: micron
 */

#include <Interfaces/Drivers/CANopenStack.h>

CANopenStack::CANopenStack(uint16_t node_id = 0){
    this->node_id = node_id;
    this->sdo_request_id = SDO_REQUEST_ADDRESS_COBID + node_id;
    this->sdo_response_id = SDO_RESPONSE_ADDRESS_COBID + node_id;

}
StateMachine_StatusWord CANopenStack::readStatusword_message(can_frame_stream can_frame_stream){
    if( can_frame_stream.data1 == 0x41 || can_frame_stream.data2 == 0x60 ){
            uint16_t statusword = ( can_frame_stream.data5 << 8 ) | ( can_frame_stream.data4 );
            this->target_reached = statusword & 0b0000010000000000;
            this->current_state = (StateMachine_StatusWord)(statusword & 0b01101111);
        }
    return this->current_state;
}
can_frame_stream requestStatusWordMessage(){
    //TODO: wtf does the 0x41 mean??
    can_frame_stream message{
        0x40, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00
    };



    return message;

}

/**
 *
 * @param command
 * @param target_id default is 0 and it means that the command is for all nodes
 * @return
 */
can_frame_stream CANopenStack::nmt_message(NMT_COMMANDS command, uint16_t target_id=0)
{
    can_frame_stream message;
    message.data0 = command;
    message.data1 = target_id;
    return message;
}
//TODO: Implementar
CANopenStack::~CANopenStack(){
    // TODO Auto-generated destructor stub
}

CANopenStack::CANopenStack(const CANopenStack &other){
    // TODO Auto-generated constructor stub

}

CANopenStack::CANopenStack(CANopenStack &&other){
    // TODO Auto-generated constructor stub

}

CANopenStack& CANopenStack::operator=(const CANopenStack &other){
   return *this;

}

CANopenStack& CANopenStack::operator=(CANopenStack &&other){
    return *this;
}

