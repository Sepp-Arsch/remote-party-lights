
#include <LoRa.h>
#include <SPI.h>

// Variables
int maxDataSize = 32; // in Bytes

// Testing (if true, per-second alternating colors)
bool testMode = false;
String firstTestColor = "199000099";
String secndTestColor = "100990099";

// Main data container
String serialData = "";

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
    delay(1000);
    if(serialData != firstTestColor) {
      serialData = firstTestColor;
    } else {
      serialData = secndTestColor;
    }
  } else if(Serial.available() > 0) {
    serialData = Serial.readString();
  }

  transmit();
}

// Transmit function sending serialData via LoRa
void transmit() {
  if(!LoRa.beginPacket()) {
    Serial.println("[FAIL] Begin LoRa Packet");
    return;
  }
  
  char buffer[32];
  serialData.toCharArray(buffer, 32);
  LoRa.write(buffer);

  if(!LoRa.endPacket()) {
    Serial.println("[FAIL] End LoRa Packet");
  }
}

// Reporting function being run when a LoRa transmission was successfull
void successfulTransmission() {
  Serial.println("[OK]   Sent: " + serialData);
}
