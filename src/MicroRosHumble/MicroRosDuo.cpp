/*
 * MicroRosDuo.cpp
 *
 *  Created on: 02/11/2023
 *      Author: micron
 */

#include "MicroRosDuo.h"
#include "bsp_api.h"
#include "common_data.h"
//TODO: THIS IS INCREDIBLY DANGEROUS AND WILL FUCK UP AT SOME POINT
class RosIntanceSingleton {
	public:
		static bool handle_open(struct uxrCustomTransport * transport){
			return RosIntanceSingleton::getInstance().handle->_transport_open(transport);
		}
		static bool handle_close(struct uxrCustomTransport * transport){
			return RosIntanceSingleton::getInstance().handle->_transport_close(transport);

		}
		static size_t handle_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* err){
			return RosIntanceSingleton::getInstance().handle->_transport_read(transport,buf, len, timeout,err);
		}
		static size_t handle_write(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * err){
			return RosIntanceSingleton::getInstance().handle->_transport_write(transport,buf, len,err);
		}
		static RosIntanceSingleton& getInstance(){
			static RosIntanceSingleton instance;
			return instance;
		}
		void setHandle(MicroRosDuo * handle){
			this->handle=handle;
		}

	private:
		RosIntanceSingleton(RosIntanceSingleton const&);
				void operator=(RosIntanceSingleton const&);
		RosIntanceSingleton(){}
		MicroRosDuo * handle;

};

MicroRosDuo::MicroRosDuo() {

	RosIntanceSingleton::getInstance().setHandle(this);
	this->remote_addr =  custom_transport_args{

							   .agent_ip_address=IP_ADDRESS(192,168,1,103),
							   .agent_port=8888
							};
				auto ret = rmw_uros_set_custom_transport(
						   false,
						   (void *) &remote_addr,
						   RosIntanceSingleton::getInstance().handle_open,
						   RosIntanceSingleton::getInstance().handle_close,
						   RosIntanceSingleton::getInstance().handle_write,
						   RosIntanceSingleton::getInstance().handle_read);
	//this->_transport_open(nullptr);

}

MicroRosDuo::~MicroRosDuo() {
	// TODO Auto-generated destructor stub
}
void* MicroRosDuo::recv(void * data, uint32_t stream_size){
	return this->EthDuo::recv(data, stream_size);
}
bool MicroRosDuo::_transport_open(struct uxrCustomTransport * transport){
    (void) transport;
    int ret_init= this->EthDuo::initialization();
    if(ret_init != 0 ){
    	// TODO: SD CARD LOG FAILURE
    	return false;
    }


    UINT status = nx_udp_socket_create(&g_ip0, &socket, (CHAR *)"Micro socket", NX_IP_NORMAL, NX_DONT_FRAGMENT, NX_IP_TIME_TO_LIVE, SOCKET_FIFO_SIZE);

  /*TODO: FIGURE OUT QOS
   *   if(NX_SUCCESS != status)
    {
        return false;
    }*/

    // Find first avaliable udp port and bind socket.
    UINT port;
    status += nx_udp_free_port_find(&g_ip0, 1, &port);
    /*TODO: FIGURE OUT QOS
    if(NX_SUCCESS != status)
    {
        return false;
    }
    */
    status += nx_udp_socket_bind(&socket, port, TX_NO_WAIT);
    /*TODO: FIGURE OUT QOS
    if(NX_SUCCESS != status)
    {
        return false;
    }
    */
   this->error_counter+=status;
    return true;
}

size_t MicroRosDuo::_transport_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* err){
    (void) transport;
    (void) len;

    NX_PACKET *data_packet = NULL;
    ULONG bytes_read = 0;

    UINT result = nx_udp_socket_receive(&socket, &data_packet, TX_MS_TO_TICKS(timeout));

    if (result != NX_SUCCESS)
    {
        *err = (uint8_t) result;
        return 0;
    }

    // Retrieve the data from the packet
    result = nx_packet_data_retrieve(data_packet, buf, &bytes_read);

    // Release the packet
    nx_packet_release(data_packet);

    if(NX_SUCCESS != result)
    {
        *err = (uint8_t) result;
        return 0;
    }

    return bytes_read;
}
bool MicroRosDuo::_transport_close(struct uxrCustomTransport * transport){
    (void) transport;

    UINT status = nx_udp_socket_unbind(&socket);

    if(NX_SUCCESS != status)
    {
        return false;
    }

    status = nx_udp_socket_delete(&socket);

    if(NX_SUCCESS != status)
    {
        return false;
    }

    status = nx_packet_pool_delete(&g_packet_pool0);

    if(NX_SUCCESS != status)
    {
        return false;
    }

    return true;
}

size_t MicroRosDuo::_transport_write(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * err){
    custom_transport_args * args = (custom_transport_args*) transport->args;

    volatile int result = FSP_SUCCESS;
    NX_PACKET *data_packet;

    // Get free packet from g_packet_pool0
    result = nx_packet_allocate(&g_packet_pool0, &data_packet, NX_UDP_PACKET, NX_NO_WAIT);

    if (result != NX_SUCCESS)
    {
        *err = (uint8_t) result;
        return 0;
    }

    result = nx_packet_data_append(data_packet, (VOID *)buf, len, &g_packet_pool0, NX_NO_WAIT);

    if (result != NX_SUCCESS)
    {
        *err = (uint8_t) result;
        return 0;
    }

    result = nx_udp_socket_send(&socket, data_packet, args->agent_ip_address, args->agent_port);

    if (result != NX_SUCCESS)
    {
        *err = (uint8_t) result;
        return 0;
    }

    return len;
}

uint32_t	MicroRosDuo::write(void *data, uint32_t stream_size){
	return this->EthDuo::write(data, stream_size);
}
int  MicroRosDuo::initialization(){
	int ret_base = EthDuo::initialization();

	return ret_base;
}

