/*============================================================================
Name        : cockpit_transmitter.ino
Author      : Yavuz Murat TAS
Version     : 1.0.0
Description : Measuring Both Temperature and Pressure by BMP180 Sensor with I2C
Note        : This program should be compiled with the directive -lm
              because of pow function in math.h to execute as desired
Sources     : https://github.com/adafruit/Adafruit_BMP085_Unified/blob/master/examples/sensorapi/sensorapi.pde for BMP180
              https://lastminuteengineers.com/mpu6050-accel-gyro-arduino-tutorial/ for MPU6050
              https://lastminuteengineers.com/dht11-module-arduino-tutorial/ for DHT11
              https://devreyakan.com/arduino-ile-mcp2515-canbus-spi-modulu-kullanimi/ for MCP2515
              https://mytectutor.com/how-to-use-mcp2515-can-module-with-arduino/ for MCP2515
============================================================================*/

/*==============================================================================
                              Arduino Uno Pins
================================================================================
BMP180 Pins
VCC                                 3V3 (POWER)
GND                                 GND (POWER)
SCL                                 SCL (DIGITAL)
SDA                                 SDA (DIGITAL)

MPU6050 Pins
VCC                                 3V3 (POWER)
GND                                 GND (POWER)
SCL                                 A5  (ANALOG)
SDA                                 A4  (ANALOG)

DHT11 Pins
VCC                                 3V3 (Power)
GND                                 GND (POWER)
SIGNAL                              8   (DIGITAL)

MCP2515 SPI-CANBUS Module
INT                                 2   (DIGITAL)
SCK                                 13  (DIGITAL)
SI                                  11  (DIGITAL)
SO                                  12  (DIGITAL)
CS                                  10  (DIGITAL)
GND                                 3V3 (POWER)
VCC                                 GND (POWER)

SPDT Switch Pins
Middle Pin                                  3V3
One of Side Pins                            A0

Blue LEDs Pins
BLED_1:                                     D7
BLED_2:                                     D6
BLED_3:                                     D5
BLED_4:                                     D4

===============================================================================*/

#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <dht.h>
#include <SPI.h>
#include <mcp2515.h>

#define MAX_ALTITUDE 1000
#define MIN_ALTITUDE 100
#define MAX_PRESS 1020
#define MIN_PRESS 1010
#define MAX_TEMP 30
#define MIN_TEMP 20
#define MAX_HUMIDITY 80
#define MIN_HUMIDITY 60
#define MAX_G 2

#define MPU6050_SCL   A5
#define MPU6050_SDA   A4

#define DHT11_SIGNAL  8

#define MCP2515_pin   10

const int MPU6050_addr = 0x68;

/* Sensors instances are created */
Adafruit_MPU6050 mpu6050;
Adafruit_BMP085_Unified bmp180 = Adafruit_BMP085_Unified(10085);
dht dht11;

/* CANBUS Messages are created */
struct can_frame canMsg1;
struct can_frame canMsg2;

#define CANMSG1ID 0x36
#define CANMSG2ID 0x56

#define S_PIN   A0

#define BLED_1  7
#define BLED_2  6
#define BLED_3  5
#define BLED_4  4

/* SPI-CANBUS MCP2515 device instance is created */
MCP2515 mcp2515(MCP2515_pin);

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  SPI.begin();

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz
  mcp2515.setNormalMode();

  if(!bmp180.begin()){
    /* BMP180 Sensor cannot begin */
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  }
  if(!mpu6050.begin()){
    /* MPU6050 Sensor cannot begin */
  }
    // set accelerometer range to +-8G
  mpu6050.setAccelerometerRange(MPU6050_RANGE_8_G);

  // set gyro range to +- 500 deg/s
  mpu6050.setGyroRange(MPU6050_RANGE_500_DEG);

  // set filter bandwidth to 21 Hz
  mpu6050.setFilterBandwidth(MPU6050_BAND_21_HZ);

  canMsg1.can_id = CANMSG1ID;
  canMsg1.can_dlc = 6;

  canMsg2.can_id = CANMSG2ID;
  canMsg2.can_dlc = 5;

  /* SPDT Switch is set to be INPUT */
  pinMode(S_PIN, INPUT);

  /* BLUE LEDs are set to be OUTPUT */
  pinMode(BLED_1, OUTPUT);
  pinMode(BLED_2, OUTPUT);
  pinMode(BLED_3, OUTPUT);
  pinMode(BLED_4, OUTPUT);

  delay(100); 
}

void loop()
{
  int switch_value;
  sensors_event_t event;
  float temp_bmp;
  float altitude;
  float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
  bmp180.getEvent(&event);
  bmp180.getTemperature(&temp_bmp);
  altitude = bmp180.pressureToAltitude(seaLevelPressure, event.pressure);

  int bmp180_temp = temp_bmp;
  int bmp180_press = event.pressure;
  int bmp180_alt = altitude;
  
  int dht11_read_data = dht11.read11(DHT11_SIGNAL);
  int dht11_temperature = dht11.temperature;
  int dht11_humidity = dht11.humidity; 

  /* MPU6050 values a for acceleration, g for gyroscope, and temp for temperature */
  sensors_event_t a, g, temp;
  mpu6050.getEvent(&a, &g, &temp);

  switch_value = analogRead(S_PIN);

  canMsg1.data[0] = a.acceleration.x + 10;
  canMsg1.data[1] = a.acceleration.y + 10;
  canMsg1.data[2] = a.acceleration.z + 10;
  canMsg1.data[3] = g.gyro.x + 10;
  canMsg1.data[4] = g.gyro.y + 10;
  canMsg1.data[5] = g.gyro.z + 10;

  canMsg2.data[0] = bmp180_press - 1000;
  canMsg2.data[1] = bmp180_alt + 200;
  canMsg2.data[2] = dht11_temperature;
  canMsg2.data[3] = dht11_humidity;
  canMsg2.data[4] = switch_value;

  if (mcp2515.sendMessage(&canMsg1) == MCP2515::ERROR_OK)
  {
    Serial.println("canMsg1 is sent");
  }
  else
  {
    Serial.println("canMsg1 error");
  }
          
  if (mcp2515.sendMessage(&canMsg2) == MCP2515::ERROR_OK)
  {
    Serial.println("canMsg2 is sent");
  }
  else
  {
    Serial.println("canMsg2 error");
  }

  if(bmp180_alt >= MAX_ALTITUDE)
  {
    digitalWrite(BLED_1, LOW);
    delay(1000);
  }
  else if(bmp180_alt <= MIN_ALTITUDE)
  {
    digitalWrite(BLED_1, HIGH);
    delay(1000);
  }
  else
  {
    digitalWrite(BLED_1, LOW);
  }

  if(bmp180_press >= MAX_PRESS)
  {
    digitalWrite(BLED_2, LOW);
    delay(100);
  }
  else if(bmp180_press <= MIN_PRESS)
  {
    digitalWrite(BLED_2, HIGH);
    delay(100);
  }
  else
  {
    digitalWrite(BLED_2, LOW);
  }

  if(dht11_temperature >= MAX_TEMP)
  {
    digitalWrite(BLED_3, LOW);
    delay(100);
  }
  else if(dht11_temperature <= MIN_TEMP)
  {
    digitalWrite(BLED_3, HIGH);
    delay(100);
  }
  else
  {
    digitalWrite(BLED_3, LOW);
  }

  if(dht11_humidity >= MAX_HUMIDITY)
  {
    digitalWrite(BLED_4, LOW);
    delay(100);
  }
  else if(dht11_humidity <= MIN_HUMIDITY)
  {
    digitalWrite(BLED_4, HIGH);
    delay(100);
  }
  else
  {
    digitalWrite(BLED_4, LOW);
  }

  Serial.print("Accl X:");
  Serial.print(a.acceleration.x);
  Serial.print(" Y:");
  Serial.print(a.acceleration.y);
  Serial.print(" Z:");
  Serial.print(a.acceleration.z);
  Serial.println();

  Serial.print("Gyro X:");
  Serial.print(g.gyro.x);
  Serial.print(" Y:");
  Serial.print(g.gyro.y);
  Serial.print(" Z:");
  Serial.print(g.gyro.z);
  Serial.println();
  Serial.print("Press:  ");
  Serial.print(bmp180_press);
  Serial.println();
  Serial.print("Altitude:  ");
  Serial.print(bmp180_alt);
  Serial.println();
  Serial.print("Temperature:  ");
  Serial.print(dht11_temperature);
  Serial.println();
  Serial.print("Humidity:  ");
  Serial.print(dht11_humidity);
  Serial.println();

  delay(2000);  

}
