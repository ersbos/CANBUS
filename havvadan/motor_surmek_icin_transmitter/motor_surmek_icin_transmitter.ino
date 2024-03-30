#include <DIYables_IRcontroller.h> // DIYables_IRcontroller library
#include <CAN.h>

#define TX_GPIO_NUM   17// Connects to CTX // Not Rx pin directly Tx pin // 7 ve 8e bağlama bozuluyor 
#define RX_GPIO_NUM  16// Connects to CRX // Not Tx pin directly Rx pin  

#define IR_RECEIVER_PIN 19 // The ESP32 pin GPIO19 connected to IR controller

DIYables_IRcontroller_17 irController(IR_RECEIVER_PIN, 200); // debounce time is 200ms


void setup() {
Serial.begin(9600);
  irController.begin();
  
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

void loop() {
  //IR
  Key17 key = irController.getKey();
  if (key != Key17::NONE) {
    switch (key) {
      case Key17::KEY_UP:
        Serial.println("UP");
        CAN.beginPacket (0x15);  //sets the ID and clears the transmit buffer
        CAN.write ('6'); //write data to buffer. data is not sent until endPacket() is called.
        CAN.endPacket();
      break;

      case Key17::KEY_DOWN:
        Serial.println("DOWN");
        CAN.beginPacket (0x15);  //sets the ID and clears the transmit buffer
        CAN.write ('5');
        CAN.endPacket();
      break;

      case Key17::KEY_LEFT:
        Serial.println("LEFT");
        CAN.beginPacket (0x15);  //sets the ID and clears the transmit buffer
        CAN.write ('4');
        CAN.endPacket();
        break;

      case Key17::KEY_RIGHT:
        Serial.println("RIGHT");
        CAN.beginPacket (0x15);  //sets the ID and clears the transmit buffer
        CAN.write ('3');
        CAN.endPacket();
      break;

      case Key17::KEY_OK :
        Serial.println("OK");
        CAN.beginPacket (0x15);
        CAN.write ('2');
        CAN.endPacket();
        break;

      default:
        Serial.println("WARNING: undefined key:");
        break;
    }
  }
}

