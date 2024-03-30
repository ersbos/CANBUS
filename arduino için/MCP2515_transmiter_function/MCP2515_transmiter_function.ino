#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
MCP2515 mcp2515(10);


void setup() {
  Serial.begin(9600);
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  Serial.println("Transmitter initialized");
}

void loop() {
  // Mesajı gönderme fonksiyonunu çağır
  sendCanMessage(0x043, 8, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02);

  delay(1000);
}

// Mesajı gönderen fonksiyon
void sendCanMessage(uint32_t id, uint8_t length, uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5, uint8_t data6, uint8_t data7) {
  canMsg.can_id  = id;
  canMsg.can_dlc = length;
  canMsg.data[0] = data0;
  canMsg.data[1] = data1;
  canMsg.data[2] = data2; 
  canMsg.data[3] = data3;
  canMsg.data[4] = data4;
  canMsg.data[5] = data5;
  canMsg.data[6] = data6;
  canMsg.data[7] = data7;

  Serial.print("Sending Data: ");
  for (int i = 0; i < canMsg.can_dlc; i++) {
    Serial.print(canMsg.data[i]);
    Serial.print(" ");
  }
  Serial.println();

  mcp2515.sendMessage(&canMsg);
}
