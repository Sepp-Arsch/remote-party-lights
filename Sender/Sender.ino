
#include <LoRa.h>
#include <SPI.h>

// General variables
String inputString; // a String to hold incoming data
int inputSize = 22; // in Bytes

// Testing variables
String testCommando = "99"; // NULL commando code
int testTime = 1000; // in seconds
String firstTestColor = "0199009999000000009900"; // Set magenta
String secndTestColor = "0100999999000000009900"; // SET cyan

// Class variables (do not change)
bool testToggle = true;

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
  if(Serial.available() >= inputSize) {
    inputString = Serial.readString();
  } else if(inputString == NULL) {
    return;
  }

  if(inputString.substring(0,2) == testCommando) {
    // if testing code, perform alternating colors scheme
    delay(testTime);
    if(testToggle) {
      transmit(firstTestColor);
    } else {
      transmit(secndTestColor);
    }
    testToggle = !testToggle;
  } else {
    //  sending actual code
    transmit(inputString);
  }
}

// Transmit function sending inputString via LoRa
void transmit(String code) {
  if(!LoRa.beginPacket()) {
    Serial.println("[FAIL] Begin LoRa Packet");
    return;
  }
  
  char buffer[inputSize];
  code.toCharArray(buffer, inputSize);
  LoRa.print(buffer);

  if(!LoRa.endPacket()) {
    Serial.println("[FAIL] End LoRa Packet");
  }
}

// Reporting function being run when a LoRa transmission was successfull
void successfulTransmission() {
  Serial.println("[OK]   Sent: " + inputString);
}
