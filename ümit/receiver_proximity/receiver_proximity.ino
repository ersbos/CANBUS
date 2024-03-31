#include <CAN.h> // Include CAN library
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcdekranim(0x27,16,2);

#define TX_GPIO_NUM   17// Connects to CTX // Not Rx pin directly Tx pin
#define RX_GPIO_NUM  16 // Connects to CRX // Not Tx pin directly Rx pin

uint8_t buf[8];

//==================================================================================//

void setup() {
  Serial.begin(115200); // Initialize serial communication
  
CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);
  CAN.begin(500E3); // Initialize CAN at 500kbps
 lcdekranim.init();
  lcdekranim.backlight();
  lcdekranim.setCursor(0,0);

}

void loop() {
   if (CAN.parsePacket()) {
    // Read the received packet
    uint32_t canId = CAN.packetId();
    uint8_t len = CAN.packetDlc();
  
    
    // Copy the data into buf
    CAN.readBytes(buf, len);

    // Print the received data
    Serial.print("Received packet from ID 0x");
    Serial.print(canId, HEX);
    Serial.print(", Data: ");
    for(int i = 0; i < len-1; i++) {
      Serial.print((char)buf[i]);
    }
    Serial.println();
  
 lcdekranim.setCursor(8,1);
 for(int i=0;i<len-1;i++){
 lcdekranim.print((char)buf[i]);
 
 }
 lcdekranim.print("cm ");
}
}


