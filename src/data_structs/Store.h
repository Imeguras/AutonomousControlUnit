/*
 * Store.h
 *
 *  Created on: 14/12/2023
 *      Author: micron
 */
#include "data_structs/AutomataStructs.hpp"
#ifndef DATA_STRUCTS_STORE_H_
#define DATA_STRUCTS_STORE_H_

namespace store {
class Store{

public:
    static Store& getInstance(){
        static Store instance;
        return instance;
    }
private:
    Store(){
            critical_autonomous_system_status={OFF};
        }

public:
    Store(Store const&)=delete;
    void operator=(Store const&)=delete;

    wrapper_int8<critical_as_state> critical_autonomous_system_status;

};


}
/* namespace store */

#endif /* DATA_STRUCTS_STORE_H_ */
