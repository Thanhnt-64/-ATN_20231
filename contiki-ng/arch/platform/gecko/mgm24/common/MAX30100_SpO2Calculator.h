#ifndef MAX30100_SPO2CALCULATOR_H
#define MAX30100_SPO2CALCULATOR_H

#include <stdint.h>


#define CALCULATE_EVERY_N_BEATS         3

struct SpO2Calculator {
    void (*SpO2Calculator)();
    void (*update)(float irACValue, float redACValue, bool beatDetected);
    void (*reset)();
    uint8_t (*getSpO2)();
};

#endif
