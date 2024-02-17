#include <math.h>

#include "MAX30100_SpO2Calculator.h"

// SaO2 Look-up Table
// http://www.ti.com/lit/an/slaa274b/slaa274b.pdf
static float irACValueSqSum;
static float redACValueSqSum;
static uint8_t beatsDetectedNum;
static uint32_t samplesRecorded;
static uint8_t spO2;

static void SpO2Calculator();
static void update(float irACValue, float redACValue, bool beatDetected);
static void reset();
static uint8_t getSpO2();

static const spO2LUT[43] = {100,100,100,100,99,99,99,99,99,99,98,98,98,98,
                                             98,97,97,97,97,97,97,96,96,96,96,96,96,95,95,
                                             95,95,95,95,94,94,94,94,94,93,93,93,93,93};

static void SpO2Calculator() {
    irACValueSqSum = 0;
    redACValueSqSum = 0;
    beatsDetectedNum = 0;
    samplesRecorded = 0;
    spO2 = 0;
}

static void update(float irACValue, float redACValue, bool beatDetected)
{
    irACValueSqSum += irACValue * irACValue;
    redACValueSqSum += redACValue * redACValue;
    ++samplesRecorded;

    if (beatDetected) {
        ++beatsDetectedNum;
        if (beatsDetectedNum == CALCULATE_EVERY_N_BEATS) {
            float acSqRatio = 100.0 * log(redACValueSqSum/samplesRecorded) / log(irACValueSqSum/samplesRecorded);
            uint8_t index = 0;

            if (acSqRatio > 66) {
                index = (uint8_t)acSqRatio - 66;
            } else if (acSqRatio > 50) {
                index = (uint8_t)acSqRatio - 50;
            }
            reset();

            spO2 = spO2LUT[index];
        }
    }
}

static void reset()
{
    samplesRecorded = 0;
    redACValueSqSum = 0;
    irACValueSqSum = 0;
    beatsDetectedNum = 0;
    spO2 = 0;
}

static uint8_t getSpO2()
{
    return spO2;
}

extern struct SpO2Calculator spO2calculator = {
    SpO2Calculator,
    update,
    reset,
    getSpO2
};