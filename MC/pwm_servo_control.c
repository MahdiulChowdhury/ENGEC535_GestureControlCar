/*************************************************** 
  This is an example for our Adafruit 16-channel PWM & Servo driver
  PWM test - this will drive 16 PWMs in a 'wave'

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815

  These drivers use I2C to communicate, 2 pins are required to  
  interface.

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(&Wire, 0x40);
int count = 0; 
int i =264; 

#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2350
#define DEFAULT_PULSE_WIDTH   1500


void setup() {
  Serial.begin(9600);
  Serial.println("16 channel PWM test!");

  //delay(1000);
  pwm.begin();
  pwm.setPWMFreq(54);  // This is the maximum PWM frequency

  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode
  // some i2c devices dont like this so much so if you're sharing the bus, watch
  // out for this!
  Wire.setClock(400000);
  delay(5000); 
  callibration();
 
}

void callibration() 
{
  pwm.setPWM(0, 0, 530); //full backward 
  delay(1000); 
  pwm.setPWM(0, 0, 105); //full forward  
  delay(1000); 
  pwm.setPWM(0, 0, 305); //full neutral  
  delay(1000);
  pwm.setPWM(0, 0, 305); //full neutral  
  delay(1000);
}


void loop() {
  
  //pwm.setPWM(0, 0, 280); //full forward  ///too slow
  
  
  pwm.setPWM(0, 0, 260); //full forward
  pwm.setPWM(1, 0, i); 
  if ( i<= 168){
    i = 168;
  }
  i -= 20;
  delay(2000);
  /*pwm.setPWM(0, 0, 260); //full forward 
  pwm.setPWM(1, 0, pulseWidth(0)); 
  delay(5000);
  
  pwm.setPWM(0, 0, 260); //full forward 
  pwm.setPWM(1, 0, pulseWidth(90)); 
  delay(5000);*/
  
  /*pwm.setPWM(0, 0, 260); //full forward 
  pwm.setPWM(1, 0, pulseWidth(90)); 
  delay(2000);*/
  
  /*pwm.setPWM(0, 0, 260); //full forward 
  pwm.setPWM(1, 0, pulseWidth(135)); 
  delay(5000);

  pwm.setPWM(0, 0, 260); //full forward 
  pwm.setPWM(1, 0, pulseWidth(180)); 
  delay(5000);
  
  pwm.setPWM(0, 0, 260); //full forward 
  pwm.setPWM(1, 0, pulseWidth(135)); 
  delay(5000);

  pwm.setPWM(0, 0, 260); //full forward 
  pwm.setPWM(1, 0, pulseWidth(90)); 
  delay(5000);
  
  pwm.setPWM(0, 0, 260); //full forward 
  pwm.setPWM(1, 0, pulseWidth(45)); 
  delay(5000);*/
  
  /*pwm.setPWM(0, 0, 350); //full backward 
  delay(3000); 
  pwm.setPWM(0, 0, 305); //full neutral  
  delay(3000);*/
}

int pulseWidth(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * 60 * 4096);
  Serial.println(analog_value);
  return analog_value;
}


