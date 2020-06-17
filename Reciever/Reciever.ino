#include <SPI.h>
#include <LoRa.h>
#include <FastLED.h>
#include <Adafruit_NeoPixel.h>

/* Kodierung: ---------------------> XX XX XX XX XX XX XX XX XX XX
 *                                   |  |  |  |  |  |  |  |  |  |
 * 2 Stellen: Kommando Nr. 00 - 99 _/   |  |  |  |  |  |  |  |  |
 * 2 Stellen: Wert Rot     00 - 99 ____/   |  |  |  |  |  |  |  |
 * 2 Stellen: Wert Grün    00 - 99 _______/   |  |  |  |  |  |  |
 * 2 Stellen: Wert Blau    00 - 99 __________/   |  |  |  |  |  |
 * 2 Stellen: Helligkeit   00 - 99 _____________/   |  |  |  |  |
 * 2 Stellen: Zeitwert 1   00 - 99 ________________/   |  |  |  |
 * 2 Stellen: Zeitwert 2   00 - 99 ___________________/   |  |  |
 * 2 Stellen: ab ID...     00 - 99 ______________________/   |  |
 * 2 Stellen: ...bis ID    00 - 99 _________________________/   |
 * 2 Stellen: Pattern Nr.  00 - 99 ____________________________/
*/

//Kommandos:
//Name                Nr.     Beschreibung                                          Parameter (Par. 1, 2, 3 und 4, je 2 Stellen)
#define C_Leer        99    //Kein (neues) Kommando                                 Kein Parameter
#define C_Ein         1     //LEDs laut Parameter einschalten                       Rot, Grün, Blau, Helligkeit
#define C_Aus         2     //LEDs aus                                              Kein Parameter
#define C_Blink       3     //LEDs Blinken                                          Rot, Grün, Blau, Interval
#define C_Blink_t     4     //LEDs Blinken + zufällige Startverzögerung             Rot, Grün, Blau, Interval
#define C_Pulse       5     //LEDs Pulsieren                                        Rot, Grün, Blau, Interval
#define C_Pulse_t     6     //LEDs Pulsieren + zufällige Startverzögerung           Rot, Grün, Blau, Interval
#define C_Flash       7     //LEDs Blitzen                                          Rot, Grün, Blau, Interval
#define C_Flash_r     8     //LEDs Blitzen zufällig                                 Rot, Grün, Blau, Interval
#define C_Ein_r       9     //LEDs bekommen zufällige Farbe                         Kein Parameter
#define C_Farbe      10     //Farb- und Helligkeitwerte in Speicher schreiben       Rot, Grün, Blau, Helligkeit
#define C_Pulse_r    11     //LEDs Pulsieren in zufälligen Farben & Zeiten          -, -, -, Interval
#define C_a          12     //
#define C_b          13     //
#define C_c          14     //
#define C_d          15     //
#define C_e          16     //
#define C_f          17     //
#define C_g          18     //
#define C_h          19     //

//Config:
#define ID            1     //ID des Ballon / Arduino
#define LEDPin        6     //LED Pin am Arduino
#define LEDCount     20     //Länge des LED Streifen
#define debounce   2000     //Cooldownzeit in ms für einzelne Befehle
#define Resolution    2     //Auflösung beim Faden

//Objekte:
Adafruit_NeoPixel strip(LEDCount, LEDPin, NEO_GRB + NEO_KHZ800);


//Variabeln:
unsigned long previousMillis = 0; 
unsigned long currentMillis = millis();
int command_t = 0;        //Kommando Timestamp
int commandreset_t = 1000;//Kommandoblockade Wirkungszeit

unsigned long Input = 0;  //Input vom Empfangsmodul
String inputString;       //Input vom Empfangsmodul als String
int par_1 = 0;            //Parameter 1
int par_2 = 0;            //Parameter 2
int par_3 = 0;            //Parameter 3
int par_4 = 0;            //Parameter 4

int val_r = 0;            //Rotwert
int val_g = 0;            //Grünwert
int val_b = 0;            //Blauwert
int val_h = 0;            //Helligkeitswert

bool LEDState = LOW;
int LEDBrightness = 0;
int interval = 0;         //Intervalzeit
int interval_r = 0;       //Zufallsinterval
int T_Flash = 100;        //Blitzdauer in ms
int command = 99;         //Aktuelles Kommando
int CommandBuffer = 99;   //Neues Kommando 


//##############################################################################################
// SETUP
//##############################################################################################

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  strip.begin();
  strip.fill(strip.Color(0, 255, 0)); //    |
  strip.setBrightness(100);           //    |
  strip.show();                       //    | STARTSIGNAL GRÜN
  delay(500);                         //    |
  strip.clear();                      //    |
  strip.show();                       //    |
}

//##############################################################################################
// LOOP
//##############################################################################################

void loop() {
  //Serial.println("Loop");
  currentMillis = millis();
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    inputString = "";
    while (LoRa.available()) {
      
      char inChar = (char)LoRa.read();
      inputString += inChar;
    }
    Serial.print("Empfangen: ");
    Serial.print(inputString);
    Serial.print(", RSSI: ");
    Serial.print(LoRa.packetRssi());

    char command_1 = inputString.charAt(0);
    char command_2 = inputString.charAt(1);
    command = 0;
    command = 10 * atoi(command_1);
    command += atoi(command_2);
    Serial.print(", Kommando: ");
    Serial.println(command);
    /*
    Input = inputString.toInt();
    
    par_4 = Input % 100;
    par_3 = (Input / 100) % 100;
    par_2 = (Input / 10000) % 100;
    par_1 = (Input / 1000000) % 100;
    CommandBuffer = (Input / 100000000) % 100;
    
    if (command == C_Blink & CommandBuffer == C_Blink_t){CommandBuffer = C_Leer;} //Blockieren von doppelter Startverzögerung
    if (command == C_Pulse & CommandBuffer == C_Pulse_t){CommandBuffer = C_Leer;} // ||
    if (CommandBuffer != C_Leer){command = CommandBuffer;}
    */
  }
  /*
  if (command != 99){
    switch(command){
      case C_Ein: //1
        mapColor();
        mapHelligkeit();
        setStrip();
        command = 99;
      break;
      //##############################################################################################
      case C_Aus: //2
        strip.clear();
        strip.show();
        command = 99;
      break;
      //##############################################################################################
      case C_Blink: //3
        mapColor(); mapInterval(); val_h = 100;
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          if (LEDState == LOW) {
            setStrip(); LEDState = HIGH;
          } 
          else {
            strip.clear(); LEDState = LOW;
          }
          strip.show();
        }
      break;
      //##############################################################################################
      case C_Blink_t: //4
        mapInterval();
        previousMillis = currentMillis + random(0, interval*2);
        command = C_Blink;
      break;
      //##############################################################################################
      case C_Pulse: //5
        mapColor(); mapInterval();
        if (currentMillis - previousMillis >= interval/(100/Resolution)) {
          previousMillis = currentMillis;
          
          if (LEDState == LOW) {LEDBrightness += Resolution;} 
          else {LEDBrightness -= Resolution;}
          
          if (LEDBrightness >= 100){LEDState = HIGH;}
          if (LEDBrightness <= 10){LEDState = LOW;}
          val_h = LEDBrightness;
          setStrip();
        }
      break;
      //##############################################################################################
      case C_Pulse_t: //6
        mapInterval();
        previousMillis = currentMillis + random(0, interval*2*100/Resolution);
        command = C_Pulse;
      break;
      //##############################################################################################
      case C_Flash: //7
        mapColor(); 
        mapInterval();
        if (currentMillis - previousMillis >= interval && LEDState == LOW) {
          previousMillis = currentMillis;
          val_h = 100; setStrip(); LEDState = HIGH;
        }
        if (currentMillis - previousMillis >= T_Flash && LEDState == HIGH) {
          strip.clear(); strip.show(); LEDState = LOW;
        }  
      break;
      //##############################################################################################
      case C_Flash_r: //8
        mapColor();
        mapInterval();
        if (currentMillis - previousMillis >= interval_r & LEDState == LOW) {
          previousMillis = currentMillis;
          val_h = 100; setStrip(); LEDState = HIGH;
          interval_r = random(interval / 10, interval);
        }
        if (currentMillis - previousMillis >= T_Flash & LEDState == HIGH) {
          strip.clear(); strip.show(); LEDState = LOW;
        }  
      break;
      //##############################################################################################
      case C_Ein_r: //9
        if (currentMillis - previousMillis >= debounce){
          previousMillis = currentMillis;
          randomColor(); val_h = 100; setStrip();
          command = 99;
        }
      break;
      //##############################################################################################
      case C_Farbe: //10
        mapColor(); mapHelligkeit();
      break;
      //##############################################################################################
      case C_Pulse_r: //11
        mapInterval();
        if (currentMillis - previousMillis >= interval_r/(100/Resolution)) {
          previousMillis = currentMillis;
          if (LEDState == LOW) {LEDBrightness += Resolution;} 
          else                 {LEDBrightness -= Resolution;}
          if (LEDBrightness >= 100){LEDState = HIGH;}
          if (LEDBrightness <= 10){LEDState = LOW; randomColor(); interval_r = random(interval / 10, interval);}
          val_h = LEDBrightness;
          setStrip();
        }
      break;
    }
  }
  */
}

void mapColor(){
  val_r = map(par_1, 0, 99, 0, 255);
  val_g = map(par_2, 0, 99, 0, 255); 
  val_b = map(par_3, 0, 99, 0, 255);
}

void randomColor(){
  val_r = random(0, 4); val_g = random(0, 4); val_b = random(0, 4);
  val_r = map(val_r, 0, 3, 0, 255);
  val_b = map(val_b, 0, 3, 0, 255);
  val_g = map(val_g, 0, 3, 0, 255);
}

void mapHelligkeit(){
  val_h = map(par_4, 0, 99, 0, 100);
}

void mapInterval(){
  interval = map(par_4, 0, 99, 200, 10000);
}

void setStrip(){
  strip.clear(); 
  strip.fill(strip.Color(val_r, val_g, val_b)); 
  strip.setBrightness(val_h); 
  strip.show();
}
