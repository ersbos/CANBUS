#include <SPI.h> //Library for using SPI Communication
#include <mcp2515.h> //Library for using CAN Communication (https://github.com/autowp/arduino-mcp2515/)

struct can_frame CanMsg; // You can write anything you want instead of "CanMsg"
/*
struct can_frame is a structure used in the SocketCAN framework for working with CAN (Controller Area Network) messages. The SocketCAN framework is a set of CAN drivers and a networking stack that allows the Linux kernel and user-space applications to communicate over a CAN bus.

In your Arduino code, you are using the can_frame structure provided by the mcp2515 library. This structure is used to represent a CAN message, and it typically includes the following fields:

can_id: The CAN identifier for the message.
can_dlc: The data length code, specifying the number of bytes in the data field.
data[8]: An array representing the data field of the CAN message. It can hold up to 8 bytes of data.
Here's a breakdown of the structure in your code:

cpp
Copy code
struct can_frame {
  uint32_t can_id;   // CAN message identifier
  uint8_t  can_dlc;  // Data Length Code (number of bytes in data field)
  uint8_t  data[8];  // Data field of the CAN message (up to 8 bytes)
};
In the context of your CAN communication code:

canMsg.can_id: This field is used to specify the CAN message identifier (ID). It's set to 0x036 in your code.
canMsg.can_dlc: This field specifies the length of the data field in bytes. It's set to 8 in your code.
canMsg.data[8]: This is an array representing the data field of the CAN message. You populate it with your custom data.
For example, in the transmitter code, you set custom data values (customData1 and customData2) in the data array of the canMsg structure. In the receiver code, you read and print the received data.

Understanding this structure is crucial when working with CAN communication using the mcp2515 library or any other library that uses the SocketCAN framework. It allows you to create, send, and receive CAN messages in your Arduino code.

*/

MCP2515 mcp2515(10); // SPI CS Pin 10
void ReceiveMessages(int CAN_ID);
int ReceivedMessages[8];


void setup() {
  Serial.begin(9600); //Begins Serial Communication at 9600 baudrate
  SPI.begin(); //Begins SPI communication
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz
  mcp2515.setNormalMode(); //Sets CAN at normal mode
  Serial.println("Receiver initialized");
}

void loop() {
 


  
  ReceiveMessages(0x43);
  Serial.println("");
  Serial.println(ReceivedMessages[5]);

}



void ReceiveMessages(int CAN_ID){

if (mcp2515.readMessage(&CanMsg) == MCP2515::ERROR_OK) {
  if (CanMsg.can_id == CAN_ID){
    Serial.println("Message received successfully");
    
    //for print out the messages
    Serial.print("Data received: ");
    for (int i = 0; i < CanMsg.can_dlc; i++) {
      ReceivedMessages[i] = CanMsg.data[i];
      Serial.print(CanMsg.data[i]);
      Serial.print(" ");
    }
    Serial.println();
    
   

  } 
  else {
    Serial.println("Error receiving message");
  }
}
  delay(1000);


 

return 0;
}







