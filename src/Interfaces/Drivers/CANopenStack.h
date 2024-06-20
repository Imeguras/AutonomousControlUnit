/*
 * CANopenStack.h
 *
 *  Created on: 20/06/2024
 *      Author: micron
 *      data values and some structures come from a repo https://github.com/Hapyr/Epos4-on-zephyr
 *      But otherwise the structure on how it operates is different
 */
//
#include <stdint.h>
#ifndef INTERFACES_DRIVERS_CANOPENSTACK_H_
#define INTERFACES_DRIVERS_CANOPENSTACK_H_
typedef union{
    uint8_t data[8];
    struct
    {
        uint8_t data0;
        uint8_t data1;
        uint8_t data2;
        uint8_t data3;
        uint8_t data4;
        uint8_t data5;
        uint8_t data6;
        uint8_t data7;
    };
}can_frame_stream;

enum NMT_COMMANDS
{
    NMT_START_REMOTE_NODE = 0x01,
    NMT_STOP_REMOTE_NODE = 0x02,
    NMT_ENTER_PRE_OPERATIONAL = 0x80,
    NMT_RESET_NODE = 0x81,
    NMT_RESET_COMMUNICATION = 0x82
};
    /**
    * @addtogroup CANopen_Object_Dictionary
    */
    enum StateMachine_StatusWord
    {
        Not_ready_to_switch_on  = 0b00000000,
        Switch_on_disabled      = 0b01000000,
        Ready_to_switch_on      = 0b00100001,
        Switched_on             = 0b00100011,
        Operation_enabled       = 0b00100111,
        Quick_stop_active       = 0b00000111,
        Fault_reaction_active   = 0b00001111,
        Fault                   = 0b00001000,
        UNKNOWN
    };

    /**
    * @addtogroup CANopen_Object_Dictionary
    */
    enum Device_Control_Commands
    {
        dcc_shutdown = 0x06,            /**< will end in axis state >ready to switch on< */
        dcc_switchon = 0x07,            /**< will end in axis state >switched on< */
        dcc_switchon_and_enable = 0x0F, /**< will end in axis state >switch on< then auto transition to >operation enabled< */
        dcc_disablevoltage = 0x00,      /**< will end in axis state >switch on disabled< */
        dcc_quickstop = 0x02,           /**< will end in axis state >quick stop active< */
        dcc_disableoperation = 0x07,    /**< will end in axis state >switched on< */
        dcc_enableoperation = 0x0F,     /**< will end in axis state >operation enabled< */
        dcc_faultreset = 0x80           /**< will end in axis state >Fault< and/or >switch on disabled< */
    };

class CANopenStack {
public:
    CANopenStack(uint16_t node_id);
    virtual ~CANopenStack();
    CANopenStack(const CANopenStack &other);
    CANopenStack(CANopenStack &&other);
    CANopenStack& operator=(const CANopenStack &other);
    CANopenStack& operator=(CANopenStack &&other);

    uint16_t g_nodeId() const;
    StateMachine_StatusWord g_currentState() const;


    can_frame_stream nmt_message(NMT_COMMANDS command, uint16_t target_id);
    can_frame_stream requestStatusWordMessage() const;
    can_frame_stream requestControlWordMessage(unsigned char highByte, unsigned char lowByte) const;
    StateMachine_StatusWord readStatusword_message(can_frame_stream);

protected:
    uint16_t node_id;
    //TODO: this is a future implementation
    uint16_t acuity_node_id;
    uint16_t sdo_request_id;
    uint16_t sdo_response_id;
    StateMachine_StatusWord current_state;
private:
    //TODO THE DUCK is this? is it a CiA thing or a CanOpen thing
    bool target_reached;

};
//DEFINiTION STUFF

#define SDO_REQUEST_ADDRESS_COBID 0x600
#define SDO_RESPONSE_ADDRESS_COBID 0x580
#define NMT_ADDRESS_COBID 0x000

#endif /* INTERFACES_DRIVERS_CANOPENSTACK_H_ */
