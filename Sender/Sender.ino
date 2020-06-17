
#include <LoRa.h>
#include <SPI.h>

<<<<<<< HEAD
unsigned long SerialData = 0; 
String  Test = "This text is used to test transmitting and recieving long strings, just like this one!";
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
=======
// Variables
int maxDataSize = 32; // in Bytes
>>>>>>> c8a269a9596837cc6d86c9399e0f3c31f4d924c1

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
  //inputString.reserve(64);

  //Continous transmit for testing purposes:
  while (true) {
    LoRa.beginPacket();
    LoRa.print(Test);
    LoRa.endPacket();
    delay(2000);
  }
}

// Loop function being continuously run when Arduino is powered on
void loop() {
<<<<<<< HEAD
  if (stringComplete) {
    Serial.print("Sending: "); Serial.println(SerialData);
    LoRa.beginPacket();
    LoRa.print(inputString);
    //LoRa.print(Test);
    //LoRa.print("da geht noch mehr Jungs");
    LoRa.endPacket();
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  /*
   while (true) {
    Serial.println("ROT");
    LoRa.beginPacket();
    LoRa.print(199000099);
    LoRa.endPacket();
    delay(1000);
    Serial.println("BLAU");
    LoRa.beginPacket();
    LoRa.print(100990099);
    LoRa.endPacket();
=======
  if(testMode) {
>>>>>>> c8a269a9596837cc6d86c9399e0f3c31f4d924c1
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
