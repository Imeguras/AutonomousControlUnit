/*
 * ethernet_logic.cpp
 *
 *  Created on: 30/09/2023
 *      Author: João Vieira
 */

#include "bsp_api.h"
#include "common_data.h"

#include <stdint.h>
#ifndef ETHDUO_H_
#define ETHDUO_H_

class EthDuo {
public:
  EthDuo();
  UINT  error_counter=0;
  virtual ~EthDuo();
  void forceInitialization();
private:


  /* Stack memory for g_ip0. */
  void g_ip0_quick_setup();
  void g_packet_pool0_quick_setup();
  /* Packet pool instance (If this is a Trustzone part, the memory must be
   * placed in Non-secure memory). */

};

#endif /* ETHDUO_H_ */
