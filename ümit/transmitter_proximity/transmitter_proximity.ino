#include <CAN.h> // Include CAN library
#include <NewPing.h>
#include <string>

#define MAX_DISTANCE 200

const int TRIGGER_PIN = 18; // Ultrasonic sensor trigger pin
const int ECHO_PIN = 5; // Ultrasonic sensor echo pin
long duration;
String stringValue;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
#define TX_GPIO_NUM   21// Connects to CTX // Not Rx pin directly Tx pin
#define RX_GPIO_NUM  22 // Connects to CRX // Not Tx pin directly Rx pin


void setup() {
  Serial.begin(115200); // Initialize serial communication
  pinMode(TRIGGER_PIN, OUTPUT); // Set trigger pin as output
  pinMode(ECHO_PIN, INPUT); // Set echo pin as input
  
CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);
  CAN.begin(500E3); // Initialize CAN at 500kbps
}

void loop() {
  
delay(150);
int uS = sonar.ping();
if (uS==0)
{
Serial.println("MAX: resetting sensor");
pinMode(ECHO_PIN, OUTPUT);
delay(150);
digitalWrite(ECHO_PIN, LOW);
delay(150);
pinMode(ECHO_PIN, INPUT);
delay(150);
}
else
{
Serial.print(" ");
Serial.print("Ping: ");
int distance = uS / US_ROUNDTRIP_CM;
Serial.print(uS / US_ROUNDTRIP_CM);
Serial.println("cm");
Serial.print("string:");

 stringValue = String(distance);
Serial.print(stringValue);
}
 char charValue[stringValue.length() + 1]; // +1 for null terminator
  stringValue.toCharArray(charValue, sizeof(charValue));
CAN.beginPacket(0x15);
  for(int i=0; i<stringValue.length() + 1;i++){
    
    CAN.write(charValue[i]);
  }
  CAN.endPacket();
delay(100);
}


void canSender(uint32_t packetID, const char* data, size_t dataSize) {
  Serial.print ("Sending packet ... ");

  CAN.beginPacket(packetID);  // Set the ID and clear the transmit buffer

  // Write data to buffer. Data is not sent until endPacket() is called.
  for (size_t i = 0; i < dataSize; ++i) {
    CAN.write((char)data[i]);
    Serial.println("sended data:");
     Serial.println(i);
    Serial.print(CAN.write(data[i]));
  }

  CAN.endPacket(); // Send the packet

  Serial.println ("done");

  delay (1000);
}

