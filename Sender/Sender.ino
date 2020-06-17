
#include <LoRa.h>
#include <SPI.h>

unsigned long SerialData = 0; 
String  Test = "This text is used to test transmitting and recieving long strings, just like this one!";
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  Serial.begin(9600);
  LoRa.begin(433E6);
  //while (!Serial);

  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
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

void loop() {
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
    delay(1000);
  }*/
  
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
