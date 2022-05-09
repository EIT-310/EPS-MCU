//
// Created by stormand on 04/04/2022.
//

#ifndef EPS_MCU_NV_STORE_H
#define EPS_MCU_NV_STORE_H
#include <string>
#include "FlashIAPBlockDevice.h"
#include "TDBStore.h"
#include "mbed.h"

class NVStore {
public:
    enum DATA_LOCATION {
        sm_state,
        adc_read
    };

    int write(DATA_LOCATION key, void *value);

    int read(DATA_LOCATION key, void *value);

private:
    static std::string GetKeyStr(DATA_LOCATION name);

};


#endif //EPS_MCU_NV_STORE_H
