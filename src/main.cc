#include "mbed.h"
#include "FSM.h"
#include "nv_store.h"
#include "adc_read.h"
// Create flash IAP block device

static DigitalInOut led(LED1);
NVStore nvm;
AdcRead adc;
Watchdog &watchdog=Watchdog::get_instance();

int main() {
    printf("STATE MACHINE TEST\n");
    FSM::state current_state;
    nvm.read(nvm.sm_state,&current_state);
    FSM sm(FSM::high);
    watchdog.start(WATCHDOG_TIMEOUT);


#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {

        nvm.read(nvm.sm_state, &current_state);         //Læs state fra NVM
        uint16_t adcVal = adc.read_int(adc.one);            //Læs ADC værdi
        printf("Current state: %s\n", sm.to_string().c_str());
        printf("ADCVal: %d\n",adcVal);
        sm.determine_state(adcVal);                        //Bestem state ud fra den målte ADC værdi

        sm.do_state();                                            //Tænd og sluk for PR alt efter hvilken state
        nvm.write(nvm.sm_state, &current_state);        //Skriv current state til NVM
        watchdog.kick();                                          //Pet watchdog
        ThisThread::sleep_for(1s);                        //Deep Sleep
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