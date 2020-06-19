
#include <LoRa.h>
#include <SPI.h>

// Variables
String inputString; // a String to hold incoming data
int inputSize = 22; // in Bytes

// Testing (if true, per-second alternating colors)
bool testMode = false;
int testTime = 1000;
String firstTestColor = "199000099";
String secndTestColor = "100990099";


// Setup function being run once when Arduino is powered on
void setup() {
  Serial.begin(9600);

  if(LoRa.begin(433E6)) {
    LoRa.onTxDone(successfulTransmission); // Executed when a LoRa transmission is finished
    Serial.println("[OK]   Setup LoRa");
  } else {
    Serial.println("[FAIL] Setup LoRa");
  }
  
  Serial.setTimeout(100);
}

// Loop function being continuously run when Arduino is powered on
void loop() {
  if(testMode) {
    delay(testTime);
    if(inputString != firstTestColor) {
      inputString = firstTestColor;
    } else {
      inputString = secndTestColor;
    }
  } else if(Serial.available() >= inputSize) {
    inputString = Serial.readString();
  } else if(inputString == NULL) {
    return;
  }

  transmit();
}

// Transmit function sending inputString via LoRa
void transmit() {
  if(!LoRa.beginPacket()) {
    Serial.println("[FAIL] Begin LoRa Packet");
    return;
  }
  
  char buffer[inputSize];
  inputString.toCharArray(buffer, inputSize);
  LoRa.write(buffer);

  if(!LoRa.endPacket()) {
    Serial.println("[FAIL] End LoRa Packet");
  }
}

// Reporting function being run when a LoRa transmission was successfull
void successfulTransmission() {
  Serial.println("[OK]   Sent: " + inputString);
}
