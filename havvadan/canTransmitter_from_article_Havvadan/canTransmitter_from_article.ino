//==================================================================================//

#include <CAN.h>

#define TX_GPIO_NUM   17// Connects to CTX // Not Rx pin directly Tx pin // 7 ve 8e bağlama bozuluyor 
#define RX_GPIO_NUM  16// Connects to CRX // Not Tx pin directly Rx pin  

//==================================================================================//

void setup() {
  Serial.begin (115200);
  while (!Serial);
  delay (1000);

  Serial.println ("CAN Receiver/Receiver");

  // Set the pins
  CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);

  // start the CAN bus at 500 kbps
  if (!CAN.begin (500E3)) {
    Serial.println ("Starting CAN failed!");
    while (1);
  }
  else {
    Serial.println ("CAN Initialized");
  }
}

//==================================================================================//

void loop() {
  // canSender();
 // canReceiver();
 canSendermMultiple();
}

//==================================================================================//

void canSender() {
  // send packet: id is 11 bits, packet can contain up to 8 bytes of data
  Serial.print ("Sending packet ... ");

  CAN.beginPacket (0x15);  //sets the ID and clears the transmit buffer
  // CAN.beginExtendedPacket(0xabcdef);
  CAN.write ('9'); //write data to buffer. data is not sent until endPacket() is called.
  CAN.write ('8');
  CAN.write ('7');
  CAN.write ('6');
  CAN.write ('5');
  CAN.write ('4');
  CAN.write ('3');
  CAN.write ('2');
  CAN.endPacket();

  //RTR packet with a requested data length
  CAN.beginPacket (0x12, 3, true);
  CAN.endPacket();

  Serial.println ("done");

  delay (1000);
}

//==================================================================================//

void canReceiver() {
  // try to parse packet
  int packetSize = CAN.parsePacket();

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
        Serial.print ((char) CAN.read());
      }
      Serial.println();
    }

    Serial.println();
  }
}

void canSendermMultiple() {
  // send packet: id is 11 bits, packet can contain up to 8 bytes of data
  Serial.print ("Sending packet ... ");

  CAN.beginPacket (0x18);  //sets the ID and clears the transmit buffer
  // CAN.beginExtendedPacket(0xabcdef);
  CAN.write ('9'); //write data to buffer. data is not sent until endPacket() is called.
  CAN.write ('8');
  CAN.write ('7');
  CAN.write ('6');
  CAN.write ('5');
  CAN.write ('4');
  CAN.write ('3');
  CAN.write ('2');
  CAN.endPacket();


 CAN.beginPacket (0x19);  //sets the ID and clears the transmit buffer
  // CAN.beginExtendedPacket(0xabcdef);
  CAN.write ('10'); //write data to buffer. data is not sent until endPacket() is called.
  CAN.write ('11');
  CAN.write ('12');
  CAN.write ('13');
  CAN.write ('14');
  CAN.write ('15');
  CAN.write ('16');
  CAN.write ('17');
  CAN.endPacket();

  //RTR packet with a requested data length
  CAN.beginPacket (0x16, 3, true);
  CAN.endPacket();

  Serial.println ("done");

  delay (1000);
}


//==================================================================================//