#include <CAN.h>

#define TX_GPIO_NUM   21 // Connects to CTX
#define RX_GPIO_NUM   22 // Connects to CRX


char received_message[8];
int motor1Pin1 = 27; 
int motor1Pin2 = 26; 
int enable1Pin = 14;

const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 170;


void setup() {
 Serial.begin (115200);
 CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);
 if (!CAN.begin (500E3)) {
    Serial.println ("Starting CAN failed!");
    while (1);
  }
  else {
    Serial.println ("CAN Initialized");
  }

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(enable1Pin, pwmChannel);
  ledcWrite(pwmChannel, dutyCycle);

}

void loop() {
 can_receiver_for_motor(21,received_message);
 switch (received_message[0]) {
      case '6':
        motor_forward();
        break;
      case '5':
        motor_backward();
        break;
    }
  
  delay(100); 
}
void motor_forward(){
digitalWrite(motor1Pin1, HIGH);
digitalWrite(motor1Pin2, LOW);
  dutyCycle = 130;
  ledcWrite(pwmChannel, dutyCycle);
  delay(1000);
}

void motor_backward(){
  // Move DC motor backward
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
dutyCycle = 130;
 ledcWrite(pwmChannel, dutyCycle);
  delay(1000);
}


void can_receiver_for_motor(int id,char* data_array){
int packetSize = CAN.parsePacket();
     // try to parse packet
  
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
       data_array[i] =(char)CAN.read();
       Serial.print(data_array[i]);
        }
      }
     
    }
  }
  }
  
}