
#include <LoRa.h>
#include <SPI.h>

unsigned long  SerialData = 0; 

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
}

void loop() {
  SerialData = Serial.parseInt();
  if (SerialData > 0) {
    Serial.print("Sending: "); Serial.println(SerialData);
    LoRa.beginPacket();
    LoRa.print(SerialData);
    //LoRa.print(Test);
    //LoRa.print("da geht noch mehr Jungs");
    LoRa.endPacket();
  }
  /* while (true) {
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
