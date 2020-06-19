#include <SPI.h>
#include <LoRa.h>
#include <FastLED.h>
#include <Adafruit_NeoPixel.h>

//##############################################################################################
//# Commands:
//##############################################################################################

//Command syntax: CC|RR|GG|BB|Br|Br|Ti|Ti|ID|ID|Pa

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

//        name       no.     description                                           parameters (r= required, o= optional, -= no effect)
//                                                                                 red   green   blue  bright_max  bright_min   t_on    t_off  fromID  toID   patternID   
#define C_NULL       99    //NULL command                                          -     -       -     -           -            -       -      -        -       -          
#define C_KILL       98    //turn off every LED                                    -     -       -     -           -            -       -      -        -       -          
#define C_SET         1    //turn LEDs on                                          r     r       r     o           -            -       -      o        o       o          
#define C_OFF         2    //turn LEDs off                                         -     -       -     -           -            -       -      o        o       o          
#define C_BLINK       3    //blink LEDs                                            r     r       r     r           o            r       r      o        o       o          
#define C_BLINK_T     4    //blink LEDs with random start delay                    r     r       r     r           o            r       r      o        o       o          
#define C_PULSE       5    //pulse LEDs                                            r     r       r     o           o            r       r      o        o       o          
#define C_PULSE_T     6    //pulse LEDs with random start delay                    r     r       r     o           -            r       r      o        o       o          
#define C_FLASH       7    //flash LEDs                                            r     r       r     o           -            r       -      o        o       o          
#define C_FLASH_R     8    //flash LEDs randomly                                   r     r       r     o           -            r       r      o        o       o          
#define C_SET_R       9    //turn LEDs on at a random color                        -     -       -     o           -            r       r      o        o       o          

#define C_PULSE_R    11    //pulse LEDs with random colors and intervals           -     -       -     o           o            o       o      o        o       o        

//##############################################################################################
//# Parameters:
//##############################################################################################

//  name                   description                                           value             translates to             value   translates to
int in_command     = 99; //current command                                       0 - 99            command no. 0 - 99
int in_red         = 0;  //intensity for red                                     0 - 99            0% - 100% red             
int in_green       = 0;  //intensity for green                                   0 - 99            0% - 100% green           
int in_blue        = 0;  //intensity for blue                                    0 - 99            0% - 100% blue            
int in_bright_max  = 0;  //max. brightness                                       1 - 99            1% - 100% brightness      0       max brightness (100%)
int in_bright_min  = 0;  //min. brightness                                       1 - 99            1% - 100% brightness      0       min brightness (0%)
int in_time_on     = 0;  //on time                                               1 - 99            20ms - 10000ms            0       default time (1000ms)
int in_time_off    = 0;  //off time                                              1 - 99            20ms - 10000ms            0       default time (1000ms)
int in_fromID      = 0;  //only apply to IDs >= ..                               0 - 19            ID 0 - ID 19              
int in_toID        = 0;  //only apply to IDs <= ..                               0 - 19            ID 0 - ID 19
int in_patternID   = 0;  //only apply to IDs matching the given pattern          0 - PatternCount  ID 0 - ID PatternCount    >PatternCount -> Pattern will be ignored

int command         = 0;
int val_red         = 0;
int val_green       = 0;
int val_blue        = 0;
int val_bright_max  = 100;
int val_bright_min  = 0;
int val_bright      = 100;
int val_time_on     = 1000;
int val_time_off    = 1000;
int val_fromID      = 0;
int val_toID        = 20;
int val_patternID   = 0;

//##############################################################################################
//# Config:
//##############################################################################################

#define pin_bit_1             3
#define pin_bit_2             4
#define pin_bit_3             5
#define pin_LED               6                              //LED Pin am Arduino
#define pin_bit_4             7
#define pin_bit_5             8
#define pin_recieve           0                              //Empfängerpin am Arduino (Interrupt 0 = Pin 2)

#define LEDCount             20                              //Länge des LED Streifen
#define BallonCount          20                              //Anzahl der Ballons
#define debounce           2000                              //Cooldownzeit in ms für einzelne Befehle
#define Resolution            2                              //Auflösung beim Faden
#define T_Flash             100                              //Blitzdauer in ms


//##############################################################################################
//# Objects:
//##############################################################################################

Adafruit_NeoPixel strip(LEDCount, pin_LED, NEO_GRB + NEO_KHZ800);

//##############################################################################################
//# Variables:
//##############################################################################################

unsigned long previousMillis = 0; 
unsigned long currentMillis = millis();
int command_t = 0;        //Kommando Timestamp
int commandreset_t = 1000;//Kommandoblockade Wirkungszeit

unsigned long Input = 0;  //Input vom Empfangsmodul
String inputString;       //Input vom Empfangsmodul als String

bool LEDState = LOW;
int LEDBrightness = 0;
int interval = 0;         //Intervalzeit
int interval_r = 0;       //Zufallsinterval
int CommandBuffer = 99;   //Neues Kommando 
int ID = 0;

//##############################################################################################
//# Pattern:
//##############################################################################################

#define PatternCount 28 //number of predefined patterns

bool pattern[PatternCount][BallonCount] = {                     
//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19   //ID
{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 }, //pattern  1: every even ID
{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 }, //pattern  2: every odd ID
{ 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0 }, //pattern  3: every 3rd ID
{ 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1 }, //pattern  4: every 3rd ID NOT
{ 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1 }, //pattern  5: every 4th ID
{ 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0 }, //pattern  6: every 4th ID NOT
{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }, //pattern  7: every 5th ID
{ 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0 }, //pattern  8: every 5th ID NOT
{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0 }, //pattern  9: every 6th ID
{ 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1 }, //pattern 10: every 6th ID NOT
{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }, //pattern 11: every 7th ID
{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 }, //pattern 12: every 7th ID NOT
{ 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }, //pattern 13: every 8th ID
{ 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 }, //pattern 14: every 8th ID NOT
{ 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0 }, //pattern 15: alternating pairs of 2, starting with ID 0
{ 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1 }, //pattern 16: alternating pairs of 2, starting with ID 2
{ 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1 }, //pattern 17: alternating pairs of 3, starting with ID 0
{ 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0 }, //pattern 18: alternating pairs of 3, starting with ID 3
{ 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1 }, //pattern 19: alternating pairs of 4, starting with ID 0
{ 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 }, //pattern 20: alternating pairs of 4, starting with ID 4
{ 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 }, //pattern 21: alternating pairs of 5, starting with ID 0
{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 }, //pattern 22: alternating pairs of 5, starting with ID 5
{ 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 }, //pattern 23: alternating pairs of 6, starting with ID 0
{ 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1 }, //pattern 24: alternating pairs of 6, starting with ID 6
{ 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 }, //pattern 25: alternating pairs of 7, starting with ID 0
{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 }, //pattern 26: alternating pairs of 7, starting with ID 7
{ 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 }, //pattern 27: alternating pairs of 8, starting with ID 0
{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 }  //pattern 28: alternating pairs of 8, starting with ID 8
};

//{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, //pattern XX: all 1
//{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //pattern XX: all 0


//##############################################################################################
//# Setup:
//##############################################################################################

void setup() {
  Serial.begin(9600);
  ID = getID();
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  LED_startup();
}

//##############################################################################################
//# Loop:
//##############################################################################################

void loop() {
  //Serial.println("Loop");
  currentMillis = millis();
  int packetSize = LoRa.parsePacket();
  if (packetSize > 1) {
    inputString = "";
    while (LoRa.available()) {
      
      char inChar = (char)LoRa.read();
      inputString += inChar;
      inputString.trim();
    }
    Serial.print("Empfangen: "); Serial.print(inputString); Serial.print(", RSSI: "); Serial.print(LoRa.packetRssi());
    
    in_command     = inputString.substring(0,2).toInt();
    in_red         = inputString.substring(2,4).toInt();
    in_green       = inputString.substring(4,6).toInt();
    in_blue        = inputString.substring(6,8).toInt();
    in_bright_max  = inputString.substring(8,10).toInt();
    in_bright_min  = inputString.substring(10,12).toInt();
    in_time_on     = inputString.substring(12,14).toInt();
    in_time_off    = inputString.substring(14,16).toInt();
    in_fromID      = inputString.substring(16,18).toInt();
    in_toID        = inputString.substring(18,20).toInt();
    in_patternID   = inputString.substring(20,22).toInt();
    
    mapValues();
    
    Serial.print(", Kommando: "); Serial.print(command); Serial.print(", Rot: "); Serial.println(val_red);
  }
  
  if (command != 99){
    switch(command){
      case C_SET: //1
        val_bright = val_bright_max;
        setStrip();
        command = 99;
      break;
      //##############################################################################################
      case C_OFF: //2
        strip.clear(); strip.show();
        command = 99;
      break;
      //##############################################################################################
      case C_BLINK: //3
        if (currentMillis - previousMillis >= val_time_off && LEDState == LOW) {
          previousMillis = currentMillis;
          val_bright = val_bright_max;
          setStrip(); LEDState = HIGH;
        }
        if (currentMillis - previousMillis >= val_time_on && LEDState == HIGH) {
          strip.clear(); strip.show(); LEDState = LOW;
        }  
      break;
      //##############################################################################################
      case C_BLINK_T: //4
        previousMillis = currentMillis + random(0, val_time_on + val_time_off);
        command = C_BLINK;
      break;
      //##############################################################################################
      case C_PULSE: //5
        if (currentMillis - previousMillis >= val_time_on / ((val_bright_max - val_bright_min)/Resolution) && LEDState = LOW) {
          LEDBrightness += Resolution;
          previousMillis = currentMillis;
        }
        if (currentMillis - previousMillis >= val_time_off / ((val_bright_max - val_bright_min)/Resolution) && LEDState = HIGH) {
          LEDBrightness -= Resolution;
          previousMillis = currentMillis;
        }
        if (LEDBrightness >= val_bright_max){LEDState = HIGH;}
        if (LEDBrightness <= val_bright_min){LEDState = LOW;}
        val_bright = LEDBrightness;
        setStrip();
      break;
      //##############################################################################################
      case C_PULSE_T: //6
        previousMillis = currentMillis + random(0, (val_time_on + val_time_off) * (val_bright_max - val_bright_min) / Resolution);
        command = C_PULSE;
      break;
      //##############################################################################################
      case C_FLASH: //7
        if (currentMillis - previousMillis >= val_time_off && LEDState == LOW) {
          previousMillis = currentMillis;
          val_bright = val_bright_max; setStrip(); LEDState = HIGH;
        }
        if (currentMillis - previousMillis >= val_time_on && LEDState == HIGH) {
          strip.clear(); strip.show(); LEDState = LOW;
        }  
      break;
      //##############################################################################################
      case C_FLASH_R: //8
        if (currentMillis - previousMillis >= interval_r & LEDState == LOW) {
          previousMillis = currentMillis;
          val_bright = val_bright_max; setStrip(); LEDState = HIGH;
          interval_r = random(interval / 10, interval);
        }
        if (currentMillis - previousMillis >= T_Flash & LEDState == HIGH) {
          strip.clear(); strip.show(); LEDState = LOW;
        }  
      break;
      //##############################################################################################
      case C_SET_R: //9
        if (currentMillis - previousMillis >= debounce){
          previousMillis = currentMillis;
          randomColor(); 
          val_bright = val_bright_max; 
          setStrip();
          command = 99;
        }
      break;
      //##############################################################################################
      case C_PULSE_R: //11
        if (currentMillis - previousMillis >= interval_r/((val_bright_max - val_bright_min)/Resolution)) {
          previousMillis = currentMillis;
          if (LEDState == LOW) {LEDBrightness += Resolution;} 
          else                 {LEDBrightness -= Resolution;}
          if (LEDBrightness >= val_bright_max){LEDState = HIGH;}
          if (LEDBrightness <= val_bright_min){LEDState = LOW; randomColor(); int interval_r = random(val_time_off, val_time_on);}
          val_bright = LEDBrightness;
          setStrip();
        }
      break;
    }
  }
}


void randomColor(){
  val_red = random(0, 4); val_green = random(0, 4); val_blue = random(0, 4);
  val_red = map(val_red, 0, 3, 0, 255);
  val_blue = map(val_blue, 0, 3, 0, 255);
  val_green = map(val_green, 0, 3, 0, 255);
}


void mapValues(){
  val_red         = map(in_red, 0, 99, 0, 255);
  val_green       = map(in_green, 0, 99, 0, 255); 
  val_blue        = map(in_blue, 0, 99, 0, 255);
  val_bright_max  = map(in_bright_max, 0, 99, 0, 100);
  val_time_on     = map(in_time_on, 0, 99, 200, 10000);
  val_time_off    = map(in_time_off, 0, 99, 200, 10000);
}

void setStrip(){
  strip.clear(); 
  strip.fill(strip.Color(val_red, val_green, val_blue)); 
  strip.setBrightness(val_bright); 
  strip.show();
}

int getID(){
  int i = 0;
  pinMode(pin_bit_1, INPUT); pinMode(pin_bit_2, INPUT); pinMode(pin_bit_3, INPUT); pinMode(pin_bit_4, INPUT); pinMode(pin_bit_5, INPUT); 
  if (digitalRead(pin_bit_1)) {i += 1;}
  if (digitalRead(pin_bit_2)) {i += 2;}
  if (digitalRead(pin_bit_3)) {i += 4;}
  if (digitalRead(pin_bit_4)) {i += 8;}
  if (digitalRead(pin_bit_5)) {i += 16;}
  return i;
}

void LED_startup(){
  strip.begin();
  strip.fill(strip.Color(0, 255, 0)); strip.setBrightness(100); strip.show(); //STARTSIGNAL GRÜN
  delay(500);                                                                
  strip.clear(); strip.show();
}
