#ifndef MAX30100_H
#define MAX30100_H
#include <stdint.h>
#include <stdio.h>
#include "MAX30100_Registers.h"

#define DEFAULT_MODE MAX30100_MODE_HRONLY
#define DEFAULT_SAMPLING_RATE MAX30100_SAMPRATE_100HZ
#define DEFAULT_PULSE_WIDTH MAX30100_SPC_PW_1600US_16BITS
#define DEFAULT_RED_LED_CURRENT MAX30100_LED_CURR_50MA
#define DEFAULT_IR_LED_CURRENT MAX30100_LED_CURR_50MA
#define EXPECTED_PART_ID 0x11

typedef struct
{
    uint16_t ir;
    uint16_t red;
} SensorReadout;

int read_register(uint8_t *regData, uint8_t numbyte);
int write_register(uint8_t register);
uint8_t i2c_read(uint8_t i2c_addr, uint8_t register_addr, uint8_t* buffer, uint8_t Nbyte );
uint8_t i2c_write(uint8_t i2c_addr, uint8_t register_addr, uint8_t* buffer, uint8_t Nbyte );
 
struct MAX30100{
    bool (*begin)();
    void (*setMode)(Mode mode);
    void (*setLedsPulseWidth)(LEDPulseWidth ledPulseWidth);
    void (*setSamplingRate)(SamplingRate samplingRate);
    void (*setLedsCurrent)(LEDCurrent irLedCurrent, LEDCurrent redLedCurrent);
    void (*setHighresModeEnabled)(bool enabled);
    void (*update)();
    bool (*getRawValues)(uint16_t *ir, uint16_t *red);
    void (*resetFifo)();
    void (*startTemperatureSampling)();
    bool (*isTemperatureReady)();
    float (*retrieveTemperature)();
    void (*shutdown)();
    void (*resume)();
    uint8_t (*getPartId)();
}

#endif
