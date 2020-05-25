#include <RCSwitch.h>

RCSwitch Sender = RCSwitch();

unsigned long SendeCode = 0;

void setup() {

  Serial.begin(9600);
  Serial.setTimeout(25);
  Sender.enableTransmit(9); // Transmitter is connected to Arduino Pin #9 
  // mySwitch.setProtocol(2);
  // mySwitch.setPulseLength(320);
  // mySwitch.setRepeatTransmit(15);
  
}

void loop() {
  /* Same switch as above, but using decimal code */
  while (Serial.available()) {
    SerialData = Serial.parseInt();
    Serial.print("Empfangen: ");
    Serial.println(SerialData);
    Sender.send(SerialData, 32);
    Serial.print("Sende: ");
    Serial.println(SerialData);
  }
  
  //Serial.println("Sende");
}
