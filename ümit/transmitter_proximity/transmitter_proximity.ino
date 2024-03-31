#include <CAN.h> // Include CAN library
#include <NewPing.h>
#include <string>

#define MAX_DISTANCE 200

const int TRIGGER_PIN = 18; // Ultrasonic sensor trigger pin
const int ECHO_PIN = 5; // Ultrasonic sensor echo pin
long duration;
String stringValue;
String stringValue2;
#define TRIGGER_PIN2 26
#define ECHO_PIN2 27

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);

#define TX_GPIO_NUM   17// Connects to CTX // Not Rx pin directly Tx pin
#define RX_GPIO_NUM  16 // Connects to CRX // Not Tx pin directly Rx pin



void setup() {
  Serial.begin(115200); // Initialize serial communication
 
CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);
  CAN.begin(500E3); // Initialize CAN at 500kbps
}

void loop() {
 proximity1_sender();
proximity2_sender();
}

void proximity1_sender(){
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
void proximity2_sender(){
  delay(150);
int uS = sonar2.ping();
if (uS==0)
{
Serial.println("MAX: resetting sensor");
pinMode(ECHO_PIN2, OUTPUT);
delay(150);
digitalWrite(ECHO_PIN2, LOW);
delay(150);
pinMode(ECHO_PIN2, INPUT);
delay(150);
}
else
{
Serial.print(" ");
Serial.print("Ping: ");
int distance2 = uS / US_ROUNDTRIP_CM;
Serial.print(uS / US_ROUNDTRIP_CM);
Serial.println("cm");
Serial.print("string2:");

 stringValue2 = String(distance2);
Serial.print(stringValue2);
}
 char charValue[stringValue2.length() + 1]; // +1 for null terminator
  stringValue2.toCharArray(charValue, sizeof(charValue));
CAN.beginPacket(0x16);
  for(int i=0; i<stringValue2.length() + 1;i++){
    CAN.write(charValue[i]);
  }
  CAN.endPacket();
delay(100);
}