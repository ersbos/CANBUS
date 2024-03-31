#include <NewPing.h>
#include <CAN.h>

#define TX_GPIO_NUM   21
#define RX_GPIO_NUM   22
#define TRIGGER_PIN   5
#define ECHO_PIN      18
#define MAX_DISTANCE  300

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(115200);
  CAN.setPins(RX_GPIO_NUM, TX_GPIO_NUM);
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  } else {
    Serial.println("CAN Initialized");
  }
}

void loop() {
  delay(150);
  int uS = sonar.ping();
  if (uS == 0) {
    Serial.println("MAX: resetting sensor");
    pinMode(ECHO_PIN, OUTPUT);
    delay(150);
    digitalWrite(ECHO_PIN, LOW);
    delay(150);
    pinMode(ECHO_PIN, INPUT);
    delay(150);
  } else {
    Serial.print("Ping: ");
    int distance_cm = uS / US_ROUNDTRIP_CM;
    Serial.print(distance_cm);
    Serial.println("cm");

    // Prepare the data buffer
    uint8_t sonar_data[1];
    sonar_data[0] = distance_cm;

    // Send data over CAN bus
    CAN.beginPacket(0x15);
    int bytes_written = CAN.write(sonar_data, sizeof(sonar_data));
    CAN.endPacket();
    
    if (bytes_written > 0) {
      Serial.print("Data sent: ");
      Serial.println(distance_cm);
    } else {
      Serial.println("Failed to send data over CAN bus");
    }
  }
}
