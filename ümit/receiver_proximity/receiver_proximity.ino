#include <CAN.h> // Include CAN library
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcdekranim(0x27,16,2);

#define TX_GPIO_NUM   17// Connects to CTX // Not Rx pin directly Tx pin
#define RX_GPIO_NUM  16 // Connects to CRX // Not Tx pin directly Rx pin

uint8_t buf[8];
//==================================================================================//



char array_for_id_21[12];


void setup() {
  Serial.begin(115200); // Initialize serial communication
  
CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);
  CAN.begin(500E3); // Initialize CAN at 500kbps
 lcdekranim.init();
  lcdekranim.backlight();
  lcdekranim.setCursor(0,0);

}

void loop() {
  //can_Receiver_for_idx(21);
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
    for(int i = 0; i < len; i++) {
      Serial.print((char)buf[i]);
    }
    Serial.println();
  }
 lcdekranim.setCursor(8,1);
 for(int i=0;i<2;i++){
 lcdekranim.print((char)buf[i]);
 }
}


void can_Receiver_for_idx(int id){
     // try to parse packet
  int packetSize = CAN.parsePacket();
 if(CAN.packetId() == id){
  if (packetSize) {
    // received a packet
    Serial.print ("Received ");

    if (CAN.packetExtended()) {
      Serial.print ("extended ");
    }

    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      Serial.print ("RTR ");
    }

    Serial.print ("packet with id 0x");
    Serial.print (CAN.packetId(), HEX);

    if (CAN.packetRtr()) {
      Serial.print (" and requested length ");
      Serial.println (CAN.packetDlc());
    } else {
      Serial.print (" and length ");
      Serial.println (packetSize);

      // only print packet data for non-RTR packets
      while (CAN.available()) {
        for(int i=0;i<packetSize;i++){
       array_for_id_21[i] =(char)CAN.read();
      // Serial.print("elementh of array ");
       //Serial.print(i);
       Serial.print(array_for_id_21[i]);
        }
      }
      Serial.println();
    }

    Serial.println();
  }
  }
}