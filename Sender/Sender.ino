#include <SPI.h>
#include <LoRa.h>

long SerialData = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  Serial.setTimeout(25);
}

void loop() {
  long tempSerialData = Serial.parseInt();
  if (tempSerialData > 0) {
    SerialData = tempSerialData;
  }

  Serial.print("Sending: "); Serial.println(SerialData);
  LoRa.beginPacket();
  LoRa.print(SerialData);
  LoRa.endPacket();

  delay(100);
}
