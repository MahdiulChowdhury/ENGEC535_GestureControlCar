
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <time.h>
//#include <math.h>
#include "i2c-dev.h"

#define PCA9685_SUBADR1 0x2 /**< i2c bus address 1 */
#define PCA9685_SUBADR2 0x3 /**< i2c bus address 2 */
#define PCA9685_SUBADR3 0x4 /**< i2c bus address 3 */

#define PCA9685_MODE1 0x0 /**< Mode Register 1 */
#define PCA9685_PRESCALE 0xFE /**< Prescaler for PWM output frequency */

#define LED0_ON_L 0x6 /**< LED0 output and brightness control byte 0 */
#define LED0_ON_H 0x7 /**< LED0 output and brightness control byte 1 */
#define LED0_OFF_L 0x8 /**< LED0 output and brightness control byte 2 */
#define LED0_OFF_H 0x9 /**< LED0 output and brightness control byte 3 */

#define ALLLED_ON_L 0xFA /**< load all the LEDn_ON registers, byte 0 */
#define ALLLED_ON_H 0xFB /**< load all the LEDn_ON registers, byte 1 */
#define ALLLED_OFF_L 0xFC /**< load all the LEDn_OFF registers, byte 0 */
#define ALLLED_OFF_H 0xFD /**< load all the LEDn_OFF registers, byte 1 */

#define ADDRESS 0x40

/* function prototypes */
void pwmBegin(int file);
void delay(int milliseconds);
void pwmReset();
void setPwmFreq(int file, int freq);
void setPwm(int file, uint8_t num, uint16_t on, uint16_t off);
uint8_t read8(int file, uint8_t reg_addr);
void write8(int file, uint8_t reg_addr, uint8_t data);


int main(){
	int fd;
	int pwmnum = 0;
	printf("i2c done\n");
	pwmBegin(fd);
	setPwm(fd, pwmnum, 0, 530);
}

void pwmBegin(int file){
	pwmReset();
	setPwmFreq(file, 54);
}

void pwmReset(int file){
	write8(file, PCA9685_MODE1, 0x80);
	delay(10);
}

void write8(int file, uint8_t reg_addr, uint8_t data){
	file = open("/dev/i2c-0", O_RDWR);

	if( ioctl(file, I2C_SLAVE, ADDRESS ) < 0)
	{
		fprintf( stderr, "Failed to set slave address: %m\n" );
	}
	
	i2c_smbus_write_byte_data(file, reg_addr, data);
	close(file);
	
}

uint8_t read8(int file, uint8_t reg_addr) {

	file = open("/dev/i2c-0", O_RDWR);

	if( ioctl(file, I2C_SLAVE, ADDRESS ) < 0)
	{
		fprintf( stderr, "Failed to set slave address: %m\n" );
	}
	
	return i2c_smbus_read_byte_data(file, reg_addr);

}


void setPwmFreq(int file, int freq){
	// setPwmFreq() --> prescale = 125  and get rid of float and floor
	/*freq *= 0.9; // Correct for overshoot in the frequency setting (see issue #11).
	float prescaleval = 25000000;
	prescaleval /= 4096;
	prescaleval /= freq;
	prescaleval -= 1;

	uint8_t prescale = floor(prescaleval + 0.5); */
	uint8_t prescale = 125;
	uint8_t oldmode = read8(file, PCA9685_MODE1);
	close(file);
	uint8_t newmode = (oldmode & 0x7F) | 0x10; // sleep
	write8(file, PCA9685_MODE1, newmode);            // go to sleep
	write8(file, PCA9685_PRESCALE, prescale);        // set the prescaler
	write8(file, PCA9685_MODE1, oldmode);
	delay(5);
	write8(file, PCA9685_MODE1, oldmode |0xa0); //  This sets the MODE1 register to turn on auto increment.

}

void setPwm(int file, uint8_t num, uint16_t on, uint16_t off){
	/*write8(file, LED0_ON_L + 4 * num);
	write8(file, on);
	write8(file, on >> 8);
	write8(file, off);
	write8(file, off >> 8);	*/
	write8(file, LED0_ON_L + 4 * num, on);
	write8(file, LED0_ON_H + 4 * num, on >> 8);
	write8(file, LED0_OFF_L + 4 * num, off);
	write8(file, LED0_OFF_H + 4 * num, off >> 8);
}

void delay(int milliseconds)
{
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}
