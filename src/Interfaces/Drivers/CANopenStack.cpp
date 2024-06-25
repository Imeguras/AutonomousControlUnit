/*
 * CANopenStack.cpp
 *
 *  Created on: 20/06/2024
 *      Author: micron
 */

#include <Interfaces/Drivers/CANopenStack.h>

CANopenStack::CANopenStack(uint16_t node_id = 0){
    this->current_state = STATUSWORD_UNKNOWN;
    this->target_reached = false;
    this->acuity_node_id = node_id;
}
StateMachine_StatusWord CANopenStack::readStatusWordMessage(can_frame_stream can_frame_stream){
    if( can_frame_stream.data1 == 0x41 || can_frame_stream.data2 == 0x60 ){
            uint16_t statusword = ( can_frame_stream.data5 << 8 ) | ( can_frame_stream.data4 );
            this->target_reached = statusword & 0b0000010000000000;
            this->current_state = (StateMachine_StatusWord)(statusword & 0b01101111);
        }
    return this->current_state;
}
std::vector<int> CANopenStack::g_bootedNodes() const{
    return this->booted_nodes;
}
void CANopenStack::a_bootedNodes(int node_id){
    this->booted_nodes.push_back (node_id);

}


StateMachine_StatusWord CANopenStack::g_currentState() const{
    return this->current_state;
}
/**
 *
 * @param command
 * @param target_id default is 0 and it means that the command is for all nodes
 * @return
 */
can_frame_stream CANopenStack::nmt_message(NMT_COMMANDS command, uint16_t target_id=0)
{
    can_frame_stream message = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    message.data0 = command;
    message.data1 = (uint8_t)target_id;

    return message;
}
can_frame_stream CANopenStack::requestControlWordMessage(unsigned char highByte, unsigned char lowByte) const{
    can_frame_stream message{0x2B, 0x40, 0x60, 0x00, lowByte, highByte, 0x00, 0x00};
    return message;

}
can_frame_stream CANopenStack::requestStatusWordMessage() const{
    //TODO: wtf does the 0x41 mean??
    can_frame_stream message{
        0x40, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00
    };



    return message;

}
uint16_t CANopenStack::g_acuityNodeId() const{
    return this->acuity_node_id;
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

