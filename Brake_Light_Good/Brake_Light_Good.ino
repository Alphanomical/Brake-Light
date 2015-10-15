/*
Code for the brake light 
amit.anand@alumni.ubc.ca
*/

//Using i2c to communicate with the ADXL345 accelerometer chip
#include <Wire.h>

//Address of the adxl345 chip, ALT ADDRESS Pin is tied to GND
#define adxlAddress 0x53

//Address of data location
#define x_data 0x32
#define y_data 0x34
#define z_data 0x36

//Values of the steady state readings from the accelerometer
//Found experimentally 
double zeroValues[3] = {115,-146,460};

//Low Pass Scaling
const float oldDataWeight = 0.95;
const float newDataWeight = 0.2;

int readingX = 0;
int prevReadingX = 0;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  
  //Constant Measurement Mode
  i2cWrite(adxlAddress,0x2D,0x08);
  //Put ADXL345 into +/- 4G Mode
  i2cWrite(adxlAddress,0x31,0x01);
}

void loop()
{
  readingX = (prevReadingX*oldDataWeight)+abs((readAccel(x_data)-zeroValues[0])*newDataWeight);
  
  prevReadingX = readingX;
  
  int brightness = map(readingX,0,400,0,255);
   
  analogWrite(6,brightness);
  
  delay(10);
}

void i2cWrite(uint8_t address, uint8_t registerAddress, uint8_t data){
  Wire.beginTransmission(address);   //start transmission to device
  Wire.write(registerAddress);       //send register address
  Wire.write(data);                  //send data to write
  Wire.endTransmission();            //end transmission
}

int readAccel(byte address) {
  uint8_t* data = i2cRead(adxlAddress,address,2);     
  return (data[0] | (data[1] << 8));
}

uint8_t* i2cRead(uint8_t address, uint8_t registerAddress, uint8_t nbytes) {
  uint8_t data[nbytes];  
  Wire.beginTransmission(address);
  Wire.write(registerAddress);
  Wire.endTransmission();  
  Wire.beginTransmission(address);
  Wire.requestFrom(address, nbytes);
  for(uint8_t i = 0; i < nbytes; i++)
    data[i] = Wire.read();
  Wire.endTransmission();  
  return data;
}
