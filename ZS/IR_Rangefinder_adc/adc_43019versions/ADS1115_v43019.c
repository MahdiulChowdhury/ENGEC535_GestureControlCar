
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h> // for O_RDWR
#include <time.h>
#include "i2c-dev.h"


/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
    #define ADS1115_ADDRESS                 0x48    // 1001 000 (ADDR = GND)
/*=========================================================================*/

/*=========================================================================
    CONVERSION DELAY (in mS)
    -----------------------------------------------------------------------*/
   // #define ADS1015_CONVERSIONDELAY         (1)
    #define ADS1115_CONVERSIONDELAY         (8)
/*=========================================================================*/

/*=========================================================================
    POINTER REGISTER
    -----------------------------------------------------------------------*/
    #define ADS1015_REG_POINTER_MASK        (0x03)
    #define ADS1015_REG_POINTER_CONVERT      0x00
    #define ADS1015_REG_POINTER_CONFIG      (0x01)
    #define ADS1015_REG_POINTER_LOWTHRESH   (0x02)
    #define ADS1015_REG_POINTER_HITHRESH    (0x03)
/*=========================================================================*/

/*=========================================================================
    CONFIG REGISTER
    -----------------------------------------------------------------------*/
    #define ADS1015_REG_CONFIG_OS_MASK      (0x8000)
    #define ADS1015_REG_CONFIG_OS_SINGLE    (0x8000)  // Write: Set to start a single-conversion
    #define ADS1015_REG_CONFIG_OS_BUSY      (0x0000)  // Read: Bit = 0 when conversion is in progress
    #define ADS1015_REG_CONFIG_OS_NOTBUSY   (0x8000)  // Read: Bit = 1 when device is not performing a conversion

    #define ADS1015_REG_CONFIG_MUX_MASK     (0x7000)
    #define ADS1015_REG_CONFIG_MUX_DIFF_0_1 (0x0000)  // Differential P = AIN0, N = AIN1 (default)
    #define ADS1015_REG_CONFIG_MUX_DIFF_0_3 (0x1000)  // Differential P = AIN0, N = AIN3
    #define ADS1015_REG_CONFIG_MUX_DIFF_1_3 (0x2000)  // Differential P = AIN1, N = AIN3
    #define ADS1015_REG_CONFIG_MUX_DIFF_2_3 (0x3000)  // Differential P = AIN2, N = AIN3
    #define ADS1015_REG_CONFIG_MUX_SINGLE_0 (0x4000)  // Single-ended AIN0
    #define ADS1015_REG_CONFIG_MUX_SINGLE_1 (0x5000)  // Single-ended AIN1
    #define ADS1015_REG_CONFIG_MUX_SINGLE_2 (0x6000)  // Single-ended AIN2
    #define ADS1015_REG_CONFIG_MUX_SINGLE_3 (0x7000)  // Single-ended AIN3

    #define ADS1015_REG_CONFIG_PGA_MASK     (0x0E00)
    #define ADS1015_REG_CONFIG_PGA_6_144V   (0x0000)  // +/-6.144V range = Gain 2/3
    #define ADS1015_REG_CONFIG_PGA_4_096V   (0x0200)  // +/-4.096V range = Gain 1
    #define ADS1015_REG_CONFIG_PGA_2_048V   (0x0400)  // +/-2.048V range = Gain 2 (default)
    #define ADS1015_REG_CONFIG_PGA_1_024V   (0x0600)  // +/-1.024V range = Gain 4
    #define ADS1015_REG_CONFIG_PGA_0_512V   (0x0800)  // +/-0.512V range = Gain 8
    #define ADS1015_REG_CONFIG_PGA_0_256V   (0x0A00)  // +/-0.256V range = Gain 16

    #define ADS1015_REG_CONFIG_MODE_MASK    (0x0100)
    #define ADS1015_REG_CONFIG_MODE_CONTIN  (0x0000)  // Continuous conversion mode
    #define ADS1015_REG_CONFIG_MODE_SINGLE  (0x0100)  // Power-down single-shot mode (default)

    #define ADS1015_REG_CONFIG_DR_MASK      (0x00E0)  
    #define ADS1015_REG_CONFIG_DR_128SPS    (0x0000)  // 128 samples per second
    #define ADS1015_REG_CONFIG_DR_250SPS    (0x0020)  // 250 samples per second
    #define ADS1015_REG_CONFIG_DR_490SPS    (0x0040)  // 490 samples per second
    #define ADS1015_REG_CONFIG_DR_920SPS    (0x0060)  // 920 samples per second
    #define ADS1015_REG_CONFIG_DR_1600SPS   (0x0080)  // 1600 samples per second (default)
    #define ADS1015_REG_CONFIG_DR_2400SPS   (0x00A0)  // 2400 samples per second
    #define ADS1015_REG_CONFIG_DR_3300SPS   (0x00C0)  // 3300 samples per second

    #define ADS1015_REG_CONFIG_CMODE_MASK   (0x0010)
    #define ADS1015_REG_CONFIG_CMODE_TRAD   (0x0000)  // Traditional comparator with hysteresis (default)
    #define ADS1015_REG_CONFIG_CMODE_WINDOW (0x0010)  // Window comparator

    #define ADS1015_REG_CONFIG_CPOL_MASK    (0x0008)
    #define ADS1015_REG_CONFIG_CPOL_ACTVLOW (0x0000)  // ALERT/RDY pin is low when active (default)
    #define ADS1015_REG_CONFIG_CPOL_ACTVHI  (0x0008)  // ALERT/RDY pin is high when active

    #define ADS1015_REG_CONFIG_CLAT_MASK    (0x0004)  // Determines if ALERT/RDY pin latches once asserted
    #define ADS1015_REG_CONFIG_CLAT_NONLAT  (0x0000)  // Non-latching comparator (default)
    #define ADS1015_REG_CONFIG_CLAT_LATCH   (0x0004)  // Latching comparator

    #define ADS1015_REG_CONFIG_CQUE_MASK    (0x0003)
    #define ADS1015_REG_CONFIG_CQUE_1CONV   (0x0000)  // Assert ALERT/RDY after one conversions
    #define ADS1015_REG_CONFIG_CQUE_2CONV   (0x0001)  // Assert ALERT/RDY after two conversions
    #define ADS1015_REG_CONFIG_CQUE_4CONV   (0x0002)  // Assert ALERT/RDY after four conversions
    #define ADS1015_REG_CONFIG_CQUE_NONE    (0x0003)  // Disable the comparator and put ALERT/RDY in high state (default)
/*=========================================================================*/


/* function prototypes */
void delay(int milliseconds);
static void writeRegister(int file, uint8_t i2cAddress, uint8_t reg, uint16_t value);
//static uint16_t readRegister(int file, uint8_t i2cAddress, uint8_t reg);
//uint16_t readADC_SingleEnded(int file, uint8_t channel);
void readRegister(int file, uint8_t i2cAddress, uint8_t reg, );
void readADC_SingleEnded(int file, uint8_t channel);

uint8_t m_i2cAddress = ADS1115_ADDRESS;
uint8_t m_conversionDelay = ADS1115_CONVERSIONDELAY;
uint8_t m_bitShift = 0;
uint16_t adc0;
// m_gain = GAIN_TWOTHIRDS; /* +/- 6.144V range (limited to VDD +0.3V max!) */

int main(){
	int fd;
	int i = 0;
	uint8_t values[2];
	printf("i2c start\n");
	while(1){
		//adc0 = readADC_SingleEnded(fd,0);
		readADC_SingleEnded(fd,1,&values);
		printf("done read\n");
		//close(fd);
		printf("i2c done\n");
		//pre43019-->printf("%d adc0 = %u\n", i, (unsigned int)adc0);
		i++;
		delay(1000);
	}
}

/* function definitions */
void delay(int milliseconds)
{
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

/**************************************************************************/
/*!
    @brief  Writes 16-bits to the specified destination register
*/
/**************************************************************************/
static void writeRegister(int file, uint8_t i2cAddress, uint8_t reg, uint16_t value) {
	/*Wire.beginTransmission(i2cAddress);
	  i2cwrite((uint8_t)reg);
	  i2cwrite((uint8_t)(value>>8));
	  i2cwrite((uint8_t)(value & 0xFF));
	  Wire.endTransmission();*/	 
	uint8_t val_array[2];
	int written_bytes;
	val_array[0] = (uint8_t)(value>>8);
	val_array[1] = (uint8_t)(value & 0xFF);
	
	file = open("/dev/i2c-0", O_RDWR);

	if( ioctl(file, I2C_SLAVE_FORCE, ADS1115_ADDRESS) < 0)
	{
		fprintf( stderr, "Failed to set slave address: %m\n" );
		printf("error\n");
	}
	
	//i2c_smbus_write_quick(file, (uint8_t)reg);
	//i2c_smbus_write_quick(file, (uint8_t)(value >> 8));
	//i2c_smbus_write_quick(file, (uint8_t)(value & 0xFF));
	//i2c_smbus_write_word_data(file, reg, value);
	written_bytes = i2c_smbus_write_block_data(file, (uint8_t)reg, (uint8_t)2, &val_array);
	written_bytes = printf("written_bytes = %d\n", written_bytes);
	printf("done write\n");
	close(file);
	/*
	file = open("/dev/i2c-0", O_RDWR);

	if( ioctl(file, I2C_SLAVE, ADS1115_ADDRESS ) < 0)
	{
		fprintf( stderr, "Failed to set slave address: %m\n" );
	}
	
	i2c_smbus_write_quick(file, (uint8_t)(value>>8));
	close(file);
	
	file = open("/dev/i2c-0", O_RDWR);

	if( ioctl(file, I2C_SLAVE, ADS1115_ADDRESS ) < 0)
	{
		fprintf( stderr, "Failed to set slave address: %m\n" );
	}
	
	i2c_smbus_write_quick(file, (uint8_t)(value & 0xFF));
	close(file); */

}

/**************************************************************************/
/*!
    @brief  Read 16-bits from the specified destination register
*/
/**************************************************************************/
void readRegister(int file, uint8_t i2cAddress, uint8_t reg, uint8_t *values) {
	 /* Wire.beginTransmission(i2cAddress);
	    i2cwrite(ADS1015_REG_POINTER_CONVERT);
	    Wire.endTransmission();
	    Wire.requestFrom(i2cAddress, (uint8_t)2);
	    return ((i2cread() << 8) | i2cread()); */
	int read_bytes; 
	file = open("/dev/i2c-0", O_RDWR);

	if( ioctl(file, I2C_SLAVE_FORCE, ADS1115_ADDRESS) < 0)
	{
		fprintf( stderr, "Failed to set slave address: %m\n" );
		printf("error\n");
	}

	i2c_smbus_write_byte(file, (uint8_t)reg);
	read_bytes = i2c_smbus_read_block_data(file, (uint8_t)reg, values);
	printf("read bytes = %d\n", read_bytes);
	printf("values[0] %u, or as int, %d \n", (unsigned int)values[0], (int)values[0]);
	printf("values[1] %u, or as int, %d \n", (unsigned int)values[1], (int)values[1]);
	printf("msb1 combined values %u,\nor as int, %d \n", (unsigned int)((values[0] << 8) | values[1]), (int)((values[0] << 8) | values[1]));
	printf("msb2 combined values %u,\nor as int, %d \n", (unsigned int)((values[1] << 8) | values[0]), (int)((values[1] << 8) | values[0]));

	//i2c_smbus_write_quick(file, (uint8_t)reg);
	//return ((((uint8_t)i2c_smbus_read_byte(file)) << 8) | ((uint8_t)i2c_smbus_read_byte(file)));
		//pre43019 -->printf("start read\n");
		//pre43019 -->i2c_smbus_write_word_data(file, reg, 0);
		//pre43019 -->adc0 = i2c_smbus_read_word_data(file, reg) >> 4;
	//close(file);
	/*close(file);
	
	file = open("/dev/i2c-0", O_RDWR);

	if( ioctl(file, I2C_SLAVE, ADS1115_ADDRESS ) < 0)
	{
		fprintf( stderr, "Failed to set slave address: %m\n" );
	}

	return ((((uint8_t)i2c_smbus_read_byte(file)) << 8) | ((uint8_t)i2c_smbus_read_byte(file)));*/

}


/**************************************************************************/
/*!
    @brief  Gets a single-ended ADC reading from the specified channel
*/
/**************************************************************************/
void readADC_SingleEnded(int file, uint8_t channel, uint8_t *values) { 
  //uint16_t adc_res;
  if (channel > 3)
  {
    //return 0;
  }
  
  // Start with default values
  uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                    ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                    ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                    ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set PGA/voltage range
  config |= ADS1015_REG_CONFIG_PGA_6_144V;

  // Set single-ended input channel
  switch (channel)
  {
    case (0):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
      break;
    case (1):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
      break;
    case (2):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
      break;
    case (3):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
      break;
  }

  // Set 'start single-conversion' bit
  config |= ADS1015_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC
  writeRegister(file, m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

  // Wait for the conversion to complete
  delay(m_conversionDelay);

  // Read the conversion results
  // Shift 12-bit results right 4 bits for the ADS1015
  //readRegister(file, m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
  readRegister(file, m_i2cAddress, ADS1015_REG_POINTER_CONVERT, values);
  //return adc_res;
}

