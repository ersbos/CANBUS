
#include <NewPing.h>
#include <CAN.h>


#define TX_GPIO_NUM   21// Connects to CTX // Not Rx pin directly Tx pin // 7 ve 8e bağlama bozuluyor 
#define RX_GPIO_NUM  22// Connects to CRX // Not Tx pin directly Rx pin  


#define TRIGGER_PIN 5
#define ECHO_PIN 18
#define MAX_DISTANCE 300


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(115200);
 CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);
  if (!CAN.begin (500E3)) {
    Serial.println ("Starting CAN failed!");
    while (1);
  }
  else {
    Serial.println ("CAN Initialized");
  }
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
Serial.print(uS / US_ROUNDTRIP_CM);
Serial.println("cm");
CAN.beginPacket(0x15);
CAN.write((char)uS / US_ROUNDTRIP_CM);
CAN.endPacket();
}

}