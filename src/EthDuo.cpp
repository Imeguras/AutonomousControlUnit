/*
 * EthDuo.cpp
 *
 *  Created on: 30/09/2023
 *      Author: micron
 */

#include "EthDuo.h"

EthDuo::EthDuo() {
	this->g_ip0_quick_setup();
	this->g_packet_pool0_quick_setup();


	    /* Initialize UART channel with baud rate 115200 */



}

EthDuo::~EthDuo() {
		// TODO Auto-generated destructor stub
}




/* Quick setup for g_ip0.
 * - g_packet_pool0 must be setup before calling this function
 *     (See Developer Assistance -> g_ip0 -> g_packet_pool0 -> Quick Setup).
 * - nx_system_initialize() must be called before calling this function.
 * - If using a NetX Duo Wireless Driver variant the quick setup for wireless must be setup (so it can connect to a network) before calling this function
 *     (See Developer Assistance -> g_ip0 -> NetX Duo Wireless Driver -> Quick Setup)
 * - If using NetX Duo with the Embedded Wireless Framework (EWF) and Cellular products then please see the FSP user manual for what to setup prior to calling this function.
 */
void EthDuo::g_ip0_quick_setup(){
    UINT status;

    /* Create the ip instance. */
    status = nx_ip_create(&g_ip0,
                "g_ip0 IP Instance",
                G_IP0_ADDRESS,
                G_IP0_SUBNET_MASK,
                &g_packet_pool0,
                G_IP0_NETWORK_DRIVER,
                &g_ip0_stack_memory[0],
                G_IP0_TASK_STACK_SIZE,
                G_IP0_TASK_PRIORITY);
        assert(NX_SUCCESS == status);

        /* If using Embedded Wireless Framework then uncomment this out to set the adapter_ptr. See the FSP user manual for more details. */
        // g_ip0.nx_ip_reserved_ptr = adapter_ptr;

        /* Set the gateway address if it is configured. Make sure this is set if using the Embedded Wireless Framework! */
        if(IP_ADDRESS(0, 0, 0, 0) != G_IP0_GATEWAY_ADDRESS)
        {
            status = nx_ip_gateway_address_set(&g_ip0, G_IP0_GATEWAY_ADDRESS);
            assert(NX_SUCCESS == status);
        }

        status = nx_arp_enable(&g_ip0, &g_ip0_arp_cache_memory[0], G_IP0_ARP_CACHE_SIZE);
        assert(NX_SUCCESS == status);

        /* TODO: Enable NetX modules that are required by the application. */

        status = nx_tcp_enable(&g_ip0);
         assert(NX_SUCCESS == status);

        status = nx_udp_enable(&g_ip0);
        assert(NX_SUCCESS == status);

        status = nx_icmp_enable(&g_ip0);
        assert(NX_SUCCESS == status);

        // status = nx_ip_fragment_enable(&g_ip0);
        // assert(NX_SUCCESS == status);

        // status = nx_igmp_enable(&g_ip0);
        // assert(NX_SUCCESS == status);

        /* Wait for the link to be enabled. */
        ULONG current_state;
        status = nx_ip_status_check(&g_ip0, NX_IP_LINK_ENABLED, &current_state, 1000U);
        assert(NX_SUCCESS == status);
        assert(NX_IP_LINK_ENABLED == current_state);
        /*nx_ip_interface_attach(&g_ip0, , ULONG ip_address, ULONG network_mask,
                                    VOID (*ip_link_driver)(struct NX_IP_DRIVER_STRUCT *));*/

}
/* Quick Setup for g_packet_pool0.
 * - nx_system_initialize() must be called before calling this function.
 */
void EthDuo::g_packet_pool0_quick_setup()
{
	/* Create the packet pool. */
	UINT status = nx_packet_pool_create(&g_packet_pool0,
				"g_packet_pool0 Packet Pool",
				G_PACKET_POOL0_PACKET_SIZE,
				&g_packet_pool0_pool_memory[0],
				G_PACKET_POOL0_PACKET_NUM * (G_PACKET_POOL0_PACKET_SIZE + sizeof(NX_PACKET)));
	assert(NX_SUCCESS == status);
}

