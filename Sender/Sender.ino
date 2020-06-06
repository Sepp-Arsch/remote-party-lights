#include <RCSwitch.h>

RCSwitch Sender = RCSwitch();

<<<<<<< Updated upstream
long SerialData = 0;
long SendeCode = 0;

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
  long tempSerialData = Serial.parseInt();
  if (tempSerialData > 0) {
    SerialData = tempSerialData;
  }
  
  Serial.print("Empfangen: "); Serial.println(SerialData);
  Sender.send(SerialData, 32);
  
  Serial.print("Sende: "); Serial.println(SerialData);
  delay(100);
=======
unsigned long SerialData = 0;

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
  }
}

void loop() {
  unsigned long tempSerialData = Serial.parseInt();
  if (tempSerialData > 0) {
    SerialData = tempSerialData;
  }

  Serial.print("Sending: "); Serial.println(SerialData);
  LoRa.beginPacket();
  LoRa.print(SerialData);
  LoRa.endPacket();

  delay(1000);
>>>>>>> Stashed changes
}
