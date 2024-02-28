#include "max30100.h"
#include "mgm240pb32vnn.h"
#include "em_i2c.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "myDriver.h"
#include "pin_config.h"
#include "em_gpio.h"
//Set up I2C, (SDA,SCL)

I2C_TypeDef *i2c;

int read_register(uint8_t *regData, uint8_t numbyte)
{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;

  seq.addr = MAX30100_ADDRESS | 0x01;
  seq.flags = I2C_FLAG_READ;

  seq.buf[0].len  = numbyte;
  seq.buf[0].data = regData;
  seq.buf[1].len  = 0;
  seq.buf[1].data = 0;

  ret = I2C_TransferInit(I2C0, &seq);
  while (ret == i2cTransferInProgress) {
      ret = I2C_Transfer(I2C0);
  }

  if (ret != i2cTransferDone) {
    // LED1 ON and infinite while loop to indicate I2C transmission problem
    GPIO_PinOutSet(gpioPortC, 3);
  }
  return 1;
}


//Wire read and write protocols
uint8_t i2c_write(uint8_t i2c_addr, uint8_t register_addr, uint8_t* buffer, uint8_t Nbyte )
{
    uint8_t ret;
    ret = I2C_LeaderWrite(i2c_addr, register_addr, buffer, Nbyte);
    return ret;
}

uint8_t i2c_read(uint8_t i2c_addr, uint8_t register_addr, uint8_t* buffer, uint8_t Nbyte )
{
    uint8_t ret;
    ret = I2C_LeaderWrite(i2c_addr, register_addr, NULL, 0);
    /* Send device address, with no STOP condition */
        /* Read data, with STOP condition  */
    //ret =  I2C_LeaderRead((i2c_addr|0x01), register_addr, buffer, Nbyte);
    read_register(buffer, Nbyte);
    return ret;
}
//

void setLEDs(pulseWidth pw, ledCurrent red, ledCurrent ir){
  uint8_t reg;
  i2c_read(MAX30100_ADDRESS, MAX30100_SPO2_CONFIG, &reg, 1);
  reg = reg & 0xFC; // Set LED_PW to 00
  reg = reg | pw;
  i2c_write(MAX30100_ADDRESS, MAX30100_SPO2_CONFIG, &reg, 1);     // Mask LED_PW
  reg = (red<<4) | ir;
  i2c_write(MAX30100_ADDRESS, MAX30100_LED_CONFIG, &reg, 1); // write LED configs
}

void setSPO2(sampleRate sr){
  uint8_t reg;
  i2c_read(MAX30100_ADDRESS, MAX30100_SPO2_CONFIG, &reg, 1);
  reg = reg & 0xE3; // Set SPO2_SR to 000
  reg = reg | (sr<<2);
  i2c_write(MAX30100_ADDRESS, MAX30100_SPO2_CONFIG, &reg, 1); // Mask SPO2_SR
  i2c_read(MAX30100_ADDRESS, MAX30100_MODE_CONFIG, &reg, 1);
  reg = reg & 0xf8; // Set Mode to 000
  reg = reg | 0x03;
  i2c_write(MAX30100_ADDRESS, MAX30100_SPO2_CONFIG, &reg, 1); // Mask MODE
}

int getNumSamp(void){
    uint8_t wrPtr;
    uint8_t rdPtr;
    i2c_read(MAX30100_ADDRESS, MAX30100_FIFO_WR_PTR, &wrPtr, 1);
    i2c_read(MAX30100_ADDRESS, MAX30100_FIFO_RD_PTR, &rdPtr, 1);
    return (abs( 16 + wrPtr - rdPtr ) % 16);
}

void setInterrupt(interruptSource intsrc)
{
  uint8_t reg;
  reg = (intsrc + 1) << 4;
  i2c_write(MAX30100_ADDRESS, MAX30100_INT_ENABLE, &reg, 1);
  i2c_read(MAX30100_ADDRESS, MAX30100_INT_STATUS, &reg, 1);
}

void setHighresModeEnabled(int enabled)
{
    uint8_t previous;
    i2c_read(MAX30100_ADDRESS, MAX30100_SPO2_CONFIG, &previous, 1);
    if (enabled == 1)
    {
      previous = previous | MAX30100_SPC_SPO2_HI_RES_EN;
      i2c_write(MAX30100_ADDRESS, MAX30100_SPO2_CONFIG, &previous, 1);
    }
    else
    {
      previous = previous & ~MAX30100_SPC_SPO2_HI_RES_EN;
      i2c_write(MAX30100_ADDRESS, MAX30100_SPO2_CONFIG, &previous, 1);
    }
}

void readSensor(hrData_t *data){
  uint8_t temp[4] = {0};  // Temporary buffer for read values
  i2c_read(MAX30100_ADDRESS, MAX30100_FIFO_DATA, temp, 4);  // Read four times from the FIFO
  data->IR = (temp[0]<<8) | temp[1];    // Combine values to get the actual number
  data->RED = (temp[2]<<8) | temp[3];   // Combine values to get the actual number
}

void shutdown(void){
  uint8_t reg;
  i2c_read(MAX30100_ADDRESS, MAX30100_MODE_CONFIG, &reg, 1);  // Get the current register
  reg = reg | 0x80;
  i2c_write(MAX30100_ADDRESS, MAX30100_MODE_CONFIG, &reg, 1);   // mask the SHDN bit
}

void reset(void){
  uint8_t reg;
  i2c_read(MAX30100_ADDRESS, MAX30100_MODE_CONFIG, &reg, 1);  // Get the current register
  reg = reg | 0x40;
  i2c_write(MAX30100_ADDRESS, MAX30100_MODE_CONFIG, &reg, 1);   // mask the RESET bit
}

void startup(void){
  uint8_t reg;
  i2c_read(MAX30100_ADDRESS, MAX30100_MODE_CONFIG, &reg, 1);  // Get the current register
  reg = reg & 0x7F;
  i2c_write(MAX30100_ADDRESS, MAX30100_MODE_CONFIG, &reg, 1);   // mask the SHDN bit
}
uint8_t getLostSample(void){
  uint8_t buffer;
  i2c_read(MAX30100_ADDRESS, MAX30100_OVRFLOW_CTR, &buffer, 1);
  return buffer;
}
uint8_t getRevID(void){
  uint8_t buffer;
  i2c_read(MAX30100_ADDRESS, MAX30100_REV_ID, &buffer, 1);
  return buffer;
}

uint8_t getPartID(void){
  uint8_t buffer;
  i2c_read(MAX30100_ADDRESS, MAX30100_PART_ID, &buffer, 1);
  return buffer;
}
void begin(pulseWidth pw, ledCurrent ir, sampleRate sr){
  uint8_t buffer;
  buffer = 0x02;
  i2c_write(MAX30100_ADDRESS, MAX30100_MODE_CONFIG, &buffer, 1); // Heart rate only
  buffer = ir;
  i2c_write(MAX30100_ADDRESS, MAX30100_LED_CONFIG, &buffer, 1);
  buffer = (sr<<2)|pw;
  i2c_write(MAX30100_ADDRESS, MAX30100_SPO2_CONFIG, &buffer, 1);
}

long meanDiff(uint16_t M) {
  #define LM_SIZE 15
  static uint16_t LM[LM_SIZE];      // LastMeasurements
  static uint8_t index = 0;
  static uint16_t sum = 0;
  static int count = 0;
  long avg = 0;

  // keep sum updated to improve speed.
  sum -= LM[index];
  LM[index] = M;
  sum += LM[index];
  index++;
  index = index % LM_SIZE;
  if (count < LM_SIZE) count++;

  avg = sum / count;
  return avg - M;
}