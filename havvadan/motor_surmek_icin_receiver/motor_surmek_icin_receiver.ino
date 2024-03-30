#include <CAN.h>

#define TX_GPIO_NUM   21 // Connects to CTX
#define RX_GPIO_NUM   22 // Connects to CRX


char received_message[8];
int motor1Pin1_Vcc = 27; 
int motor1Pin1_Gnd = 26; 
int motor1Pin2_Vcc = 25; 
int motor1Pin2_Gnd = 33; 
int enable1Pin_motor_1= 14;
int enable1Pin_motor_2= 12;

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
//motor 1
  pinMode(motor1Pin1_Vcc, OUTPUT);
  pinMode(motor1Pin1_Gnd, OUTPUT);
  pinMode(enable1Pin_motor_1, OUTPUT);
//motor 2
  pinMode(motor1Pin2_Vcc, OUTPUT);
  pinMode(motor1Pin2_Gnd, OUTPUT);
  pinMode(enable1Pin_motor_2, OUTPUT);

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
      case '4' :
        motor_left();
        break;
      case '3' :
        motor_right();
        break;
      case '2':
        stop();
        break;
    }
  delay(100); 
}

void motor_forward(){
  // Move DC motor forward
  digitalWrite(motor1Pin1_Vcc, HIGH);
  digitalWrite(motor1Pin1_Gnd, LOW);
  digitalWrite(motor1Pin2_Vcc, HIGH);
  digitalWrite(motor1Pin2_Gnd, LOW);
  analogWrite(enable1Pin_motor_1, 255/2);
  analogWrite(enable1Pin_motor_2, 255/2);
}

void motor_backward(){
  // Move DC motor backward
  digitalWrite(motor1Pin1_Vcc, LOW);
  digitalWrite(motor1Pin1_Gnd, HIGH);
  digitalWrite(motor1Pin2_Vcc, LOW);
  digitalWrite(motor1Pin2_Gnd, HIGH);
  analogWrite(enable1Pin_motor_1, 255/2);
  analogWrite(enable1Pin_motor_2, 255/2);
}

void stop(){
// Stop DC motor 
  digitalWrite(motor1Pin1_Vcc, LOW);
  digitalWrite(motor1Pin1_Gnd, LOW);
  digitalWrite(motor1Pin2_Vcc, LOW);
  digitalWrite(motor1Pin2_Gnd, LOW);
  analogWrite(enable1Pin_motor_1, 0);
  analogWrite(enable1Pin_motor_2, 0); 
}

void motor_left(){
// Move DC motor left
  digitalWrite(motor1Pin1_Vcc, HIGH);
  digitalWrite(motor1Pin1_Gnd, LOW);
  digitalWrite(motor1Pin2_Vcc, LOW);
  digitalWrite(motor1Pin2_Gnd, HIGH);
  analogWrite(enable1Pin_motor_1, 255/2);
  analogWrite(enable1Pin_motor_2, 255/2);
}

void motor_right(){
  // Move DC motor right
  digitalWrite(motor1Pin1_Vcc, LOW);
  digitalWrite(motor1Pin1_Gnd, HIGH);
  digitalWrite(motor1Pin2_Vcc, HIGH);
  digitalWrite(motor1Pin2_Gnd, LOW);
  analogWrite(enable1Pin_motor_1, 255/2);
  analogWrite(enable1Pin_motor_2, 255/2);
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