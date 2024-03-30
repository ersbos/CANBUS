/*============================================================================
Name        : cockpit_receiver.ino
Author      : Yavuz Murat TAS
Version     : 1.0.0
Description : Measuring Both Temperature and Pressure by BMP180 Sensor with I2C
Sources     : 
              https://mytectutor.com/how-to-use-mcp2515-can-module-with-arduino/ for MCP2515
              https://www.robocombo.com/blog/icerik/arduino-ile-nokia-5110-lcd-ekran-kullanimi-resim-yazdirma for Nokia 5110 LCD
              https://linuxhint.com/arduino-nano-i2c-lcd-display/ for LCD 16x2 I2C
============================================================================*/

/*==============================================================================
                              Arduino Nano Pins
================================================================================
Nokia 5110 LCD Screen Pins
RST                                         D3
CE                                          D4
DC                                          D5
DIN                                         D6
CLK                                         D7
VCC                                         3V3
BL                                          3V3
GND                                         GND

16x2 LCD Screen with I2C Converter Pins
VCC                                         3V3
GND                                         GND
SCL                                         A5
SDA                                         A4

Buzzer Pins
RED                                         9 
BLACK                                       GND

Red LEDs Pins
RLED_1:                                     D8
RLED_2:                                     A0
RLED_3:                                     A1
RLED_4:                                     A2
RLED_5:                                     A3

MCP2515 SPI-CANBUS Module

===============================================================================*/

#include <SPI.h>
#include <mcp2515.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <LiquidCrystal_I2C.h>
#include <LCD5110_Basic.h> 
#include <Wire.h>

#define MAX_ALTITUDE 1000
#define MIN_ALTITUDE 100
#define MAX_PRESS 1020
#define MIN_PRESS 1010
#define MAX_TEMP 30
#define MIN_TEMP 20
#define MAX_HUMIDITY 80
#define MIN_HUMIDITY 60
#define MAX_G 2

#define N_RST  10
#define N_CE   9
#define N_DC   8
#define N_DIN  7
#define N_CLK  6

#define LCD_SCL A5
#define LCD_SDA A4

#define B_RED   9

#define RLED_1  8
#define RLED_2  A0
#define RLED_3  A1
#define RLED_4  A2
#define RLED_5  A3

#define MCP2515_pin   10

/* CANBUS Messages are created */
struct can_frame canMsg1;
struct can_frame canMsg2;

#define CANMSG1ID 0x36
#define CANMSG2ID 0x56

/* SPI-CANBUS MCP2515 device instance is created */
MCP2515 mcp2515(MCP2515_pin);

// Declare LCD object for software SPI
// Adafruit_PCD8544(CLK,DIN,D/C,CE,RST);
LCD5110 display(7,6,5,4,3);
extern uint8_t SmallFont[];

LiquidCrystal_I2C lcd(0x27, 16, 2);  /* I2C scanned address defined + I2C screen size */

void setup()
{
  SPI.begin();
  Serial.begin(9600);
  Wire.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz
  mcp2515.setNormalMode();

  //Initialize Display
  display.InitLCD();

  // you can change the contrast around to adapt the display for the best viewing!
  display.setContrast(60);

  // Clear the buffer.
  display.clrScr();

  // Display Text
  display.setFont(SmallFont);

  lcd.init();  /* LCD display initialized */
  lcd.backlight();      /* Turn ON LCD Backlight */
  lcd.print("System Opening");
  delay(1000);
  lcd.clear();     /* Clear LCD Display */

  /* Buzzer is set to be OUTPUT */
  pinMode(B_RED, OUTPUT);

  /* RED LEDs are set to be OUTPUT */
  pinMode(RLED_1, OUTPUT);
  pinMode(RLED_2, OUTPUT);
  pinMode(RLED_3, OUTPUT);
  pinMode(RLED_4, OUTPUT);
  pinMode(RLED_5, OUTPUT);

}

void loop()
{
  int accl_x, accl_y, accl_z, gyro_x, gyro_y, gyro_z, total_g;
  int press, temp, altitude, humidity;
  int switch_value;

  if(mcp2515.readMessage(&canMsg1) == MCP2515::ERROR_OK)
  {
    Serial.println("canMsg1 eşleştirme yapıldı.");

    if(canMsg1.can_id == 0x36)
    {
      Serial.println("canMsg1");
      accl_x = canMsg1.data[0] - 10;
      accl_y = canMsg1.data[1] - 10;
      accl_z = canMsg1.data[2] - 10;
      gyro_x = canMsg1.data[3] - 10;
      gyro_y = canMsg1.data[4] - 10;
      gyro_z = canMsg1.data[5] - 10;

      delay(100);
    }
    else
    {
      Serial.println("canMsg1 id hatasi");
      /* Wrong ID */
    }
  }
  else
  {
    Serial.println("canMsg1 okunamadi.");
    /* Cannot be read */
  }

  if (mcp2515.readMessage(&canMsg2) == MCP2515::ERROR_OK)
  {
    Serial.println("canMsg2 eşleştirme yapıldı.");

    if(canMsg2.can_id == CANMSG2ID)
    {
      Serial.println("canMsg2");
      press = canMsg2.data[0] + 1000;
      altitude = canMsg2.data[1] - 200;
      temp = canMsg2.data[2];
      humidity = canMsg2.data[3];
      switch_value = canMsg2.data[4];

      delay(100);
    }
    else
    {
      Serial.println("canMsg2 id hatasi.");
      /* Wrong ID */
    }
  }
  else
  {
    Serial.println("canMsg2 okunamadi.");
    /* Cannot be read */
  }

  total_g = sqrt( pow(accl_x, 2) + pow(accl_y, 2) + pow(accl_z, 2) );

  // Display Text
  display.clrScr();
  display.print("X", 0, 0);
  display.print("Y", 40, 0);
  display.print("Z", 75, 0);
  display.print("ACC", 30, 8);
  display.printNumI(accl_x, 0, 16);
  display.printNumI(-accl_y, 35, 16);
  display.printNumI(-accl_z, 65, 16);
  display.print("GYRO", 30, 32);
  display.printNumI(gyro_x, 0, 40);
  display.printNumI(-gyro_y, 35, 40);
  display.printNumI(-gyro_z, 65, 40);

  if(switch_value > 100)
  {
    lcd.clear();
    lcd.setCursor(0, 0);   /*Set cursor to Row 1*/
    lcd.print("Temperature:"); /*print text on LCD*/
    lcd.print(temp);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity:"); /*print text on LCD*/
    lcd.print(humidity);
    delay(100);
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);   /*Set cursor to Row 1*/
    lcd.print("Pressure:"); /*print text on LCD*/
    lcd.print(press);
    lcd.print("hPa");
    lcd.setCursor(0, 1);
    lcd.print("Altitude:"); /*print text on LCD*/
    lcd.print(altitude);
    lcd.print("m");
    delay(100);
  }

  if(altitude >= MAX_ALTITUDE)
  {
    digitalWrite(RLED_1, HIGH);
    digitalWrite(B_RED, HIGH);
    delay(1000);
  }
  else if(altitude <= MIN_ALTITUDE)
  {
    digitalWrite(RLED_1, LOW);
    digitalWrite(B_RED, HIGH);
    delay(1000);
  }
  else
  {
    digitalWrite(RLED_1, LOW);

  }

  if(press >= MAX_PRESS)
  {
    digitalWrite(RLED_2, HIGH);
    digitalWrite(B_RED, HIGH);
    delay(100);
  }
  else if(press <= MIN_PRESS)
  {
    digitalWrite(RLED_2, LOW);
    digitalWrite(B_RED, HIGH);
    delay(100);
  }
  else
  {
    digitalWrite(RLED_2, LOW);
  }

  if(temp >= MAX_TEMP)
  {
    digitalWrite(RLED_3, HIGH);
    digitalWrite(B_RED, HIGH);
    Serial.println("kırmızı 1 yandı.");
    delay(100);
  }
  else if(temp <= MIN_TEMP)
  {
    digitalWrite(RLED_3, LOW);
    digitalWrite(B_RED, HIGH);
    Serial.println("mavi 1 söndü.");
    delay(100);
  }
  else
  {
    digitalWrite(RLED_3, LOW);
    Serial.println("mavi ve kırmızı söndü.");
  }

  if(humidity >= MAX_HUMIDITY)
  {
    digitalWrite(RLED_4, HIGH);
    digitalWrite(B_RED, HIGH);
    delay(100);
  }
  else if(humidity <= MIN_HUMIDITY)
  {
    digitalWrite(RLED_4, LOW);
    digitalWrite(B_RED, HIGH);
    delay(100);
  }
  else
  {
    digitalWrite(RLED_4, LOW);
  }
  
  if(total_g >= MAX_G)
  {
    digitalWrite(RLED_5, HIGH);
    digitalWrite(B_RED, HIGH);
    delay(100);
  }
  else
  {
    digitalWrite(RLED_5, LOW);
    digitalWrite(B_RED, LOW);
  }
  
  digitalWrite(B_RED, LOW);

  Serial.print("Accl X:");
  Serial.print(accl_x);
  Serial.print(" Y:");
  Serial.print(accl_y);
  Serial.print(" Z:");
  Serial.print(accl_z);
  Serial.println();

  Serial.print("Gyro X:");
  Serial.print(gyro_x);
  Serial.print(" Y:");
  Serial.print(gyro_y);
  Serial.print(" Z:");
  Serial.print(gyro_z);
  Serial.println();
  Serial.print("Press:  ");
  Serial.print(press);
  Serial.println();
  Serial.print("Altitude:  ");
  Serial.print(altitude);
  Serial.println();
  Serial.print("Temperature:  ");
  Serial.print(temp);
  Serial.println();
  Serial.print("Humidity:  ");
  Serial.print(humidity);
  Serial.println();
  Serial.print("Total g:  ");
  Serial.print(total_g);
  Serial.println();

  delay(1000);
}
