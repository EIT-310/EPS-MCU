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

int main(void) {
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
    int cursor;

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
}



