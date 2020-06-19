
#include <LoRa.h>
#include <SPI.h>

String  Test = "This text is used to test transmitting and recieving long strings, just like this one!";
String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

// Variables
int maxDataSize = 32; // in Bytes

// Testing (if true, per-second alternating colors)
bool testMode = false;
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
  //inputString.reserve(64);
}

// Loop function being continuously run when Arduino is powered on
void loop() {
  if (stringComplete) {
    Serial.print("Sending: "); Serial.println(inputString);
    LoRa.beginPacket();
    LoRa.print(inputString);
    LoRa.endPacket();
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  
  if(testMode) {
    delay(1000);
    if(inputString != firstTestColor) {
      inputString = firstTestColor;
    } else {
      inputString = secndTestColor;
    }
  } else if(Serial.available() > 0) {
    inputString = Serial.readString();
  }

  transmit();
}

// Transmit function sending inputString via LoRa
void transmit() {
  if(!LoRa.beginPacket()) {
    Serial.println("[FAIL] Begin LoRa Packet");
    return;
  }
  
  char buffer[32];
  inputString.toCharArray(buffer, 32);
  LoRa.write(buffer);

  if(!LoRa.endPacket()) {
    Serial.println("[FAIL] End LoRa Packet");
  }
}

// Reporting function being run when a LoRa transmission was successfull
void successfulTransmission() {
  Serial.println("[OK]   Sent: " + inputString);
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
