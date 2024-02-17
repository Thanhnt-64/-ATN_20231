#include "MAX30100.h"
#include "em_i2c.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "myDriver.h"
#include "em_gpio.h"
#include "gpio-hal.h"

static int bufferIR[16]={0};
static int bufferRED[16]={0};
static uint8_t indexIR = 0;
static uint8_t indexRED = 0;
static void shiftBuf(int *buff){
    for(int i = 15; i > 0; i--){
        buff[i] = buff[i] - 1;
    }
}
I2C_TypeDef *i2c;

static bool begin(unsigned long busSpeed)
{
    if (getPartId() != EXPECTED_PART_ID)
    {
        return false;
    }

    setMode(DEFAULT_MODE);
    setLedsPulseWidth(DEFAULT_PULSE_WIDTH);
    setSamplingRate(DEFAULT_SAMPLING_RATE);
    setLedsCurrent(DEFAULT_IR_LED_CURRENT, DEFAULT_RED_LED_CURRENT);
    setHighresModeEnabled(true);
    return true;
}

static void setMode(Mode mode)
{
    writeRegister(MAX30100_REG_MODE_CONFIGURATION, mode);
}

static void setLedsPulseWidth(LEDPulseWidth ledPulseWidth)
{
    uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
    writeRegister(MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xfc) | ledPulseWidth);
}

static void setSamplingRate(SamplingRate samplingRate)
{
    uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
    writeRegister(MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xe3) | (samplingRate << 2));
}

static void setLedsCurrent(LEDCurrent irLedCurrent, LEDCurrent redLedCurrent)
{
    writeRegister(MAX30100_REG_LED_CONFIGURATION, redLedCurrent << 4 | irLedCurrent);
}

static void setHighresModeEnabled(bool enabled)
{
    uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
    if (enabled)
    {
        writeRegister(MAX30100_REG_SPO2_CONFIGURATION, previous | MAX30100_SPC_SPO2_HI_RES_EN);
    }
    else
    {
        writeRegister(MAX30100_REG_SPO2_CONFIGURATION, previous & ~MAX30100_SPC_SPO2_HI_RES_EN);
    }
}

static void update()
{
    readFifoData();
}

static bool getRawValues(uint16_t *ir, uint16_t *red)
{
    if (buffer[15] != 0)
    {
        *ir = bufferIR[indexIR];
        *red = bufferIR[indexRED];
        indexIR --;
        indexRED --;
        shiftBuf(bufferIR);
        shiftBuf(bufferRED);
        return true;
    }
    else
    {
        return false;
    }
}

static void resetFifo()
{
    writeRegister(MAX30100_REG_FIFO_WRITE_POINTER, 0);
    writeRegister(MAX30100_REG_FIFO_READ_POINTER, 0);
    writeRegister(MAX30100_REG_FIFO_OVERFLOW_COUNTER, 0);
}

static void readFifoData()
{
    uint8_t buffer[MAX30100_FIFO_DEPTH * 4];
    uint8_t toRead;

    toRead = (readRegister(MAX30100_REG_FIFO_WRITE_POINTER) - readRegister(MAX30100_REG_FIFO_READ_POINTER)) & (MAX30100_FIFO_DEPTH - 1);

    if (toRead)
    {
        burstRead(MAX30100_REG_FIFO_DATA, buffer, 4 * toRead);

        for (uint8_t i = 0; i < toRead; ++i)
        {
            // Warning: the values are always left-aligned
            readoutsBuffer.push({.ir = (uint16_t)((buffer[i * 4] << 8) | buffer[i * 4 + 1]),
                                 .red = (uint16_t)((buffer[i * 4 + 2] << 8) | buffer[i * 4 + 3])});
        }
    }
}

static void startTemperatureSampling()
{
    uint8_t modeConfig = readRegister(MAX30100_REG_MODE_CONFIGURATION);
    modeConfig |= MAX30100_MC_TEMP_EN;

    writeRegister(MAX30100_REG_MODE_CONFIGURATION, modeConfig);
}

static bool isTemperatureReady()
{
    return !(readRegister(MAX30100_REG_MODE_CONFIGURATION) & MAX30100_MC_TEMP_EN);
}

static float retrieveTemperature()
{
    int8_t tempInteger = readRegister(MAX30100_REG_TEMPERATURE_DATA_INT);
    float tempFrac = readRegister(MAX30100_REG_TEMPERATURE_DATA_FRAC);

    return tempFrac * 0.0625 + tempInteger;
}

static void shutdown()
{
    uint8_t modeConfig = readRegister(MAX30100_REG_MODE_CONFIGURATION);
    modeConfig |= MAX30100_MC_SHDN;

    writeRegister(MAX30100_REG_MODE_CONFIGURATION, modeConfig);
}

static void resume()
{
    uint8_t modeConfig = readRegister(MAX30100_REG_MODE_CONFIGURATION);
    modeConfig &= ~MAX30100_MC_SHDN;

    writeRegister(MAX30100_REG_MODE_CONFIGURATION, modeConfig);
}

static uint8_t getPartId()
{
    return readRegister(0xff);
}


static uint8_t readRegister(uint8_t addr){
    uint8_t rxBuff;
    I2C_LeaderRead(MAX30100_I2C_ADDRESS, addr, &rxBuff, 1);
    return rxBuff;
}

static void writeRegister(uint8_t addr, uint8_t data){
    I2C_LeaderWrite(MAX30100_I2C_ADDRESS, addr, &data, 1);
}

static void burstRead(uint8_t addr, uint8_t *buf, uint8_t len){
    I2C_LeaderRead(MAX30100_I2C_ADDRESS, addr, buf, len);
}

const struct hrm = {
    begin,
    setMode,
    setLedsPulseWidth,
    setSamplingRate,
    setLedsCurrent,
    setHighresModeEnabled,
    update,
    getRawValues,
    resetFifo,
    startTemperatureSampling,
    isTemperatureReady,
    retrieveTemperature,
    shutdown,
    resume,
    getPartId
};