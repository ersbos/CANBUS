#include <SPI.h>
#include <mcp2515.h>

struct can_frame CanMsg;
MCP2515 mcp2515(10);

int ReceivedMessages[8];

//Engine control pins:

const int Engine_Vcc_1 = 4;
const int Engine_Gnd_1 = 5;
const int Engine_Vcc_2 = 6;
const int Engine_Gnd_2 = 7;

//engine controls function:

void forward();
void stop();
void setup() {

  // Engine control pins:
  pinMode(Engine_Vcc_1, OUTPUT);
  pinMode(Engine_Gnd_1, OUTPUT);
  pinMode(Engine_Vcc_2, OUTPUT);
  pinMode(Engine_Gnd_2, OUTPUT);


  Serial.begin(115200);
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  Serial.println("Receiver initialized");
}

void loop() {
  
    ReceiveMessages(0x43, ReceivedMessages); // Örnek CAN_ID ve dizi

    switch (ReceivedMessages[0]) {
      case 6:
        forward();
        break;
      case 4:
        // İkinci durumun işlemleri
        backward();
        break;
      case 7:
        // Üçüncü durumun işlemleri
        left();
        break;
      case 5:
        // Dördüncü durumun işlemleri
        right();
        break;
      default:
        // Belirtilen durumlar dışında bir değer alındığında yapılacak işlemler
        stop();
        break;
    }
  
  delay(100); // Veri alım hızını kontrol etmek için kısa bir gecikme
}

void ReceiveMessages(int CAN_ID, int *data_array) {
  if (mcp2515.readMessage(&CanMsg) == MCP2515::ERROR_OK) {
    if (CanMsg.can_id == CAN_ID) {
      Serial.println("Message received successfully");
      Serial.print("Data received: ");
      for (int i = 0; i < CanMsg.can_dlc; i++) {
        data_array[i] = CanMsg.data[i];
        Serial.print(data_array[i]);
        Serial.print(" ");
      }
      Serial.println();
    } else {
      Serial.println("Error receiving message");
    }
  }
}


void forward() {
  // Motor 1'i ileri yönde çalıştır
  digitalWrite(Engine_Vcc_1, HIGH);
  digitalWrite(Engine_Gnd_1, LOW);
  
  // Motor 2'yi ileri yönde çalıştır
  digitalWrite(Engine_Vcc_2, HIGH);
  digitalWrite(Engine_Gnd_2, LOW);
  
  ReceiveMessages(0x43, ReceivedMessages); // Örnek CAN_ID ve dizi

}

void stop() {
  // Motor 1'i ileri yönde çalıştır
  digitalWrite(Engine_Vcc_1, LOW);
  digitalWrite(Engine_Gnd_1, LOW);
  
  // Motor 2'yi ileri yönde çalıştır
  digitalWrite(Engine_Vcc_2, LOW);
  digitalWrite(Engine_Gnd_2, LOW);
  
  ReceiveMessages(0x43, ReceivedMessages); // Örnek CAN_ID ve dizi

}

void left() {
  // Motor 1'i ileri, Motor 2'yi geri yönde çalıştır
  digitalWrite(Engine_Vcc_1, HIGH);
  digitalWrite(Engine_Gnd_1, LOW);
  digitalWrite(Engine_Vcc_2, LOW);
  digitalWrite(Engine_Gnd_2, HIGH);
  
  ReceiveMessages(0x43, ReceivedMessages); // Örnek CAN_ID ve dizi
}

void right() {
  // Motor 1'i geri, Motor 2'yi ileri yönde çalıştır
  digitalWrite(Engine_Vcc_1, LOW);
  digitalWrite(Engine_Gnd_1, HIGH);
  digitalWrite(Engine_Vcc_2, HIGH);
  digitalWrite(Engine_Gnd_2, LOW);
  
  ReceiveMessages(0x43, ReceivedMessages); // Örnek CAN_ID ve dizi
}

void backward() {
  // Motor 1'i geri, Motor 2'yi geri yönde çalıştır
  digitalWrite(Engine_Vcc_1, LOW);
  digitalWrite(Engine_Gnd_1, HIGH);
  digitalWrite(Engine_Vcc_2, LOW);
  digitalWrite(Engine_Gnd_2, HIGH);
  
  ReceiveMessages(0x43, ReceivedMessages); // Örnek CAN_ID ve dizi
}




