/*
 * Copyright (c) 2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

// Maximum number of element the application buffer can contain
#define MAXIMUM_BUFFER_SIZE                                                  32

// Create a DigitalOutput object to toggle an LED whenever data is received.
static DigitalOut led(LED1);

// Create a BufferedSerial object with a default baud rate.
static BufferedSerial serial_port(USBTX, USBRX);
CAN can(D10,D2);
Thread thread1;
string rec_msg;
[[noreturn]] void recieve(){
    while (1) {

        CANMessage recieve_msg;

    if (can.read(recieve_msg)) {

        serial_port.write("Albert: ",8);
        serial_port.write(recieve_msg.data,recieve_msg.len);

    }
    }
}


int main() {
    // Set desired properties (9600-8-N-1).
    serial_port.set_baud(115200);
    serial_port.set_format(
            /* bits */ 8,
            /* parity */ BufferedSerial::None,
            /* stop bit */ 1
    );

    // Application buffer to receive the data
    char buf[MAXIMUM_BUFFER_SIZE] = {0};
    char msg[MAXIMUM_BUFFER_SIZE];
    uint32_t cursor;
    thread1.start(&recieve);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {
        if (uint32_t num = serial_port.read(buf, sizeof(buf))) {
            // Toggle the LED.
            led = !led;
            for (int i = 0; i < num; ++i) {
                msg[cursor + i] = buf[i];
            }
            cursor += num;
        }

        for (int i = 0; i < cursor; ++i) {
            if (msg[i] == '\n') {
                i++;
                CANMessage canMsg(1,msg,i);
                can.write(canMsg);

                serial_port.write("Jens: ",6);
                serial_port.write(msg, i);

                for (int j = 0; j < cursor-1; ++j) {
                    char tmp = msg[j + i];
                    msg[j + i] = 0;
                    msg[j] = tmp;
                }
                cursor = cursor - i;
            }
        }
    }
#pragma clang diagnostic pop
}



