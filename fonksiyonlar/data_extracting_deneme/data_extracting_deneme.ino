
#include <CAN.h>
#include <vector>

#define TX_GPIO_NUM   21// Connects to CTX // Not Rx pin directly Tx pin
#define RX_GPIO_NUM  22 // Connects to CRX // Not Tx pin directly Rx pin

// Define the maximum buffer size
#define MAX_BUFFER_SIZE 100

// Function to dynamically allocate a buffer
char* createDynamicBuffer(int bufferSize) {
  // Check if the requested buffer size exceeds the maximum buffer size
  if (bufferSize > MAX_BUFFER_SIZE) {
    Serial.println("Error: Requested buffer size exceeds maximum buffer size.");
    return NULL;
  }

  // Dynamically allocate memory for the buffer
  char* buffer = (char*)malloc(bufferSize * sizeof(char));

  // Check if memory allocation was successful
  if (buffer == NULL) {
    Serial.println("Error: Memory allocation failed.");
    return NULL;
  }

  // Return the dynamically allocated buffer
  return buffer;
}

char* receiver_deneme(int buffersize){
  int packetSize= CAN.parsePacket();
  if (packetSize) {
    std::vector<int> myVector;
      



  
      

  }
}

void setup() {
  Serial.begin(9600);

  // Example usage of the createDynamicBuffer function
  int bufferSize = 50; // Change the buffer size as needed
  char* dynamicBuffer = createDynamicBuffer(bufferSize);

  // Check if buffer creation was successful
  if (dynamicBuffer != NULL) {
    Serial.println("Dynamic buffer created successfully!");
    // Use the buffer as needed...
    
    // Free the dynamically allocated memory when it's no longer needed
    free(dynamicBuffer);
  }
}

void loop() {

  // Nothing to do here
}
