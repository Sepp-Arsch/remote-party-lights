
#include <LoRa.h>
#include <SPI.h>

// Variables
String  inputString;        // a String to hold incoming data
int     inputSize       =   22; // in Bytes


bool    testMode        =   0; // Testing (if true, per-second alternating colors)
int     testTime        =   1000;
String  firstTestColor  =   "0199000099000000009900";
String  secndTestColor  =   "0100990099000000009900";

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

void loop() {
  if(testMode) {
    delay(testTime);
    if(inputString != firstTestColor) { inputString = firstTestColor; } else { inputString = secndTestColor; }
  } 
  else if(Serial.available() >= inputSize) {
    inputString = Serial.readString();
    inputString.trim();
    Serial.print("[RECEIVED] ");
    Serial.println(inputString);
  } 
  else if(inputString == NULL) { return; }

  transmit();
  inputString = "";
  delay(10);
}

// Transmit function sending inputString via LoRa
void transmit() {
  if(!LoRa.beginPacket()) { Serial.println("[FAIL] Begin LoRa Packet"); return; }
  
  char buffer[inputSize];
  inputString.toCharArray(buffer, inputSize);
  LoRa.print(buffer);

  if(!LoRa.endPacket()) { Serial.println("[FAIL] End LoRa Packet");}
  Serial.print("[SENT] ");
  Serial.println(inputString);
}

void successfulTransmission() { // Reporting function being run when a LoRa transmission was successfull
  Serial.println("[OK]   Sent: " + inputString);
}
