#include "mbed.h"
 
AnalogIn analog_sensor(A0);                      // Create the analog pin object
 
int main()
{
    uint16_t adc_value;                          // ADC value for analog read variable
    float voltage_value;                         // Voltage value for analog read variable
    float percent_value;                         // Percent value for analog read variable
    
    while(1)
    {
        adc_value = analog_sensor.read_u16();    // Read the ADC analog input value [0 to 65535]
        // printf("Analog Read: ");                 // Show the text between " "
        printf("%d\n", adc_value);         // Show the voltage value [0 to 65535]
    
        ThisThread::sleep_for();                  // Wait 1ms between reads for stability
    }
}