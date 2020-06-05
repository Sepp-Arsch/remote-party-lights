#include <RCSwitch.h>

RCSwitch Sender = RCSwitch();

long SerialData = 0;
long SendeCode = 0;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(25);
  Sender.enableTransmit(9); // Transmitter is connected to Arduino Pin #9
  Serial.println("Sender initialized...");
}

void loop() {
  long tempSerialData = Serial.parseInt();
  if (tempSerialData > 0) {
    SerialData = tempSerialData;
  }

  Serial.print("Sending: "); Serial.println(SerialData);
  Sender.send(SerialData, 32);
  delay(100);
}
