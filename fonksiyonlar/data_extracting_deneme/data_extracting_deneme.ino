#include <CAN.h>

#define TX_GPIO_NUM 21  // Connects to CTX // Not Rx pin directly Tx pin
#define RX_GPIO_NUM 22  // Connects to CRX // Not Tx pin directly Rx pin

const int ARRAY_SIZE = 11; // Assuming a CAN message has 11 bytes of data
char receivedData[ARRAY_SIZE]; // Array to store received data

void setup() {
  Serial.begin(9600);

  // Initialize CAN bus at 500kbps
  CAN.begin(500E3);
}

void loop() {
  int packetSize = CAN.parsePacket();

  if (packetSize) {
    // Check if the received message has ID 21
    if (CAN.packetId() == 21) {
      // Read data bytes into the array
      for (int i = 0; i < ARRAY_SIZE; ++i) {
        receivedData[i] = (char)CAN.read();
      }
      
      // Print the received data
      Serial.print("Received data for ID 21: ");
      for (int i = 0; i < ARRAY_SIZE; ++i) {
        Serial.print(receivedData[i]); 
        Serial.print(" ");
      }
      Serial.println();
    }
  }

  delay(100); // Adjust delay as needed
}
