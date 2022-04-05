#include "mbed.h"
#include "FSM.h"
#include "nv_store.h"
#include "adc_read.h"
// Create flash IAP block device

static DigitalInOut led(LED1);
NVStore nvm;
AdcRead adc;
int main() {
    printf("STATE MACHINE TEST\n");
    FSM::state current_state;
    nvm.read(nvm.sm_state,&current_state);
    FSM sm(FSM::high);



#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {

        nvm.read(nvm.sm_state, &current_state);
        uint16_t adcVal = adc.read_int(adc.one);
        printf("Current state: %s\n", sm.to_string().c_str());
        printf("ADCVal: %d\n",adcVal);
        if (((float) adcVal / UINT16_MAX)*3.3 > 3.0) {
            sm.up_state();
            current_state = sm.getCurrentState();

        } else if (((float) adcVal / UINT16_MAX)*3.3 < 0.8) {
            sm.down_state();
            current_state = sm.getCurrentState();
        }

        sm.do_state();
        nvm.write(nvm.sm_state, &current_state);
        ThisThread::sleep_for(1s);
    }
#pragma clang diagnostic pop








//    sm.down_state();
//    current_state = sm.getCurrentState();
//    printf("Current state: %s\n", sm.to_string().c_str());
//    sm.do_state();
//




}

// Hej Abber Labber :)
//
//▬▬▬.◙.▬▬▬
//═▂▄▄▓▄▄▂
//◢◤ █▀▀████▄▄▄▄◢◤
//█▄ █ █▄ ███▀▀▀▀▀▀▀╬
//◥█████◤
//══╩══╩═
//╬═╬
//╬═╬
//╬═╬
//╬═╬       Hello?
//╬═╬  ☻/
//╬═╬ /▌
//╬═╬ / \