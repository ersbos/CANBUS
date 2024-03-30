#include <LiquidCrystal_I2C.h>

#include <Wire.h>

LiquidCrystal_I2C lcdekranim(0x27,16,2);

void setup() 
{
  lcdekranim.init();
  lcdekranim.backlight();
  lcdekranim.setCursor(0,0);
  lcdekranim.print("Giris Ekrani");
  delay(1000);
  lcdekranim.clear();
}

void loop()
{
 lcdekranim.setCursor(8,1);
 lcdekranim.print("hoca");
 lcdekranim.setCursor(1,0);
 lcdekranim.print("Elif Robotik");
}