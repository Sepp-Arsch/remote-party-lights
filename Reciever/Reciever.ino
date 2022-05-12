//LED party lights - Reciever program for Arduino nano.

//##############################################################################################
//# Wiring:
//##############################################################################################

//              Arduino Nano     
//               
//                  _____- [PSU] USB
//              ___| USB |___
//             |   |_____|   |
// [LoRa] SCK -|o D13   D12 o|- [LoRa] MISO
// [LoRa] VIN -|o 3v3   D11 o|- [LoRa] MOSI
//          / -|o REF   D10 o|- [LoRa] NSS
//          / -|o A0     D9 o|- [LoRa] RST
//   ID_bit_1 -|o A1     D8 o|- /
//   ID_bit_2 -|o A2     D7 o|- /
//   ID_bit_3 -|o A3     D6 o|- [LEDs] Din
//   ID_bit_4 -|o A4     D5 o|- /
//   ID_bit_5 -|o A5     D4 o|- /
//          / -|o A6     D3 o|- /
//          / -|o A7     D2 o|- [LoRa] DI00
// [LEDs] +5V -|o 5V    GND o|- [LoRa] GND
//          / -|o RST   RST o|- /
// [LEDs] GND -|o GND    RX o|- /
//          / -|o VIN    TX o|- /
//             |_____________|
//           
//      
//              LoRa Module                               
//              ____________________                                _________________
//      [ANT] -|o              GND o|- /                          -|o DI02      GND o|- [Nano] GND
//            -|o             DI01 o|- /                          -|o DI01     MISO o|- [Nano] D12 
//             |              DI02 o|- /               [Nano] D2  -|o DI00     MOSI o|- [Nano] D11
//             |              DI03 o|- /               [Nano] 3v3 -|o 3V3       SCK o|- [Nano] D13  
//             |               VCC o|- [Nano] 3v3                 -|o DI04      NSS o|- [Nano] D10  
//             |              MISO o|- [Nano] D12                 -|o DI03      RST o|- [Nano] D9
//             |              MOSI o|- [Nano] D11                 -|o GND       DO5 o|-
//             |              SLCK o|- [Nano] D13           [ANT] -|o ANA       GND o|-
//             |               NSS o|- [Nano] D10                  |_________________|
//             |              DI00 o|- [Nano] D2
//             |               RST o|- [Nano] D9
//             |               GND o|- [Nano] GND
//             |____________________|
//      
//      
//             NeoPixel LED Strip                          |<------- 20 X ------>|
//              ________________________________________________________________________________/     /_____________________________________
// [Nano] +5V -|o 5V   .-----.   5V o|o 5V   .-----.   5V o|o 5V   .-----.   5V o|o 5V   .-----/     /  .-----.   5V o|o 5V   .-----.   5V o|-
// [Nano]  D6 -|o Din  |     | Dout o|o Din  |     | Dout o|o Din  |     | Dout o|o Din  |    / ... /n  |     | Dout o|o Din  |     | Dout o|-
// [Nano] GND -|o GND  |_____|  GND o|o GND  |_____|  GND o|o GND  |_____|  GND o|o GND  |___/     /ND  |_____|  GND o|o GND  |_____|  GND o|-
//             |_____________________|_____________________|_____________________|__________/     /___________________|_____________________|
//                                                                                         /     /
//             
//             Power Supply       
//              _________________  
// [Batt] (+) -|o B+             |
//             |             ,-----.
//             |             | USB | - [Nano] USB
// [Batt] (-) -|o B-   ___   `-----´
//             |______|USB|______|
//                    |___|
//                charging port
//

//##############################################################################################
//# Config:
//##############################################################################################

//      name              value          description                           default
#define pin_bit_1            A1        //ID adress input bits                       A1
#define pin_bit_2            A2        //          ||                               A2
#define pin_bit_3            A3        //          ||                               A3
#define pin_bit_4            A4        //          ||                               A4
#define pin_bit_5            A5        //          ||                               A5
#define pin_LED               6        //digital pin for LED control                 6

#define LEDCount             20        //lenght of LED stripe                       20
#define ModuleCount          22        //number of balloons                         20
#define C_Cooldown         2000        //cooldown time for static commands        2000
#define resolution            2        //increments when fading                      2
#define T_Flash             100        //duration of a flash in ms                 100
#define trail                 5        //Number of trailing LEDs                     5


bool    DemoMode   =          0;       //demo mode for testing without transmitter   0
bool    DemoDone   =          1;       //
String  DemoString = "3399000099005000009900";
//                    CCRRGGBBbbBBttTTidIDpt

//##############################################################################################
//# Libraries & Objects:
//##############################################################################################

#include <SPI.h>
#include <LoRa.h>
#include <FastLED.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip(LEDCount, pin_LED, NEO_GRB + NEO_KHZ800);

//##############################################################################################
//# Commands:
//##############################################################################################

//  commands (C = command, SD = Random start delay, B = backwards,                         parameters (r= required, o= optional, -= no effect)
//            RT = random timing, RC = random color)
//        name              no.    description                                          |      color      | brightness|    time   |       ID        |
//                                                                                      | red  green  blue| max   min |  on   off | from  to pattern| 
#define C_NULL              99  //NULL command                                          |  -     -     -  |  -     -  |  -     -  |  -     -     -  |        
#define C_KILL              98  //turn off every LED                                    |  -     -     -  |  -     -  |  -     -  |  -     -     -  |          
#define C_SET               1   //turn LEDs on                                          |  r     r     r  |  r     -  |  -     -  |  o     o     o  |         
#define C_OFF               2   //turn LEDs off                                         |  -     -     -  |  -     -  |  -     -  |  o     o     o  |         
#define C_BLINK             3   //blink LEDs                                            |  r     r     r  |  r     o  |  r     r  |  o     o     o  |         
#define C_BLINK_SD          4   //blink LEDs with random start delay                    |  r     r     r  |  r     o  |  r     r  |  o     o     o  |         
#define C_PULSE             5   //pulse LEDs                                            |  r     r     r  |  r     o  |  r     r  |  o     o     o  |        
#define C_PULSE_SD          6   //pulse LEDs with random start delay                    |  r     r     r  |  r     -  |  r     r  |  o     o     o  |        
#define C_FLASH             7   //flash LEDs                                            |  r     r     r  |  r     -  |  r     -  |  o     o     o  |        
#define C_FLASH_RT          8   //flash LEDs randomly                                   |  r     r     r  |  r     -  |  r     r  |  o     o     o  |        
#define C_SET_RC            9   //turn LEDs on at a random color                        |  -     -     -  |  r     -  |  r     r  |  o     o     o  |        
#define C_FADE              10  //fade to specified color                               |  r     r     r  |  r     -  |  r     -  |  o     o     o  |
#define C_PULSE_RCT         11  //pulse LEDs with random colors and intervals           |  -     -     -  |  r     o  |  o     o  |  o     o     o  |     
#define C_FLAME             12  //flame effect                                          |  -     -     -  |  r     -  |  -     -  |  o     o     o  |
#define C_RAINBOW           13  //rainbow cycle effect                                  |  -     -     -  |  r     -  |  r     -  |  o     o     o  |
#define C_RAINBOW_RC        14  //rainbow cycle effect, starting with random color      |  -     -     -  |  r     -  |  r     -  |  o     o     o  |
#define C_SHIFT             15  //shifts in the new color LED by LED                    |  r     r     r  |  r     -  |  r     -  |  o     o     o  |
#define C_SHIFT_B           16  //shifts in the new color LED by LED (backwards)        |  r     r     r  |  r     -  |  r     -  |  o     o     o  |
#define C_SHIFT_RAINBOW     17  //shifts along a rainbow pattern                        |  -     -     -  |  r     -  |  r     -  |  o     o     o  |
#define C_SHIFT_RAINBOW_RC  18  //same as above, starting with random color             |  -     -     -  |  r     -  |  r     -  |  o     o     o  |
#define C_SHIFT_CHAOTIC     19  //change the LEDs one by one in a random order          |  r     r     r  |  r     -  |  r     -  |  o     o     o  |
#define C_SET_RL_RC         20  //change a random LED to a random color repeatedly      |  -     -     -  |  r     -  |  r     -  |  o     o     o  |
#define C_CHASE             21  //some LEDs chasing along the LED strip                 |  r     r     r  |  r     -  |  r     -  |  o     o     o  |
#define C_CHASE_B           22  //some LEDs chasing along the LED strip (backwards)     |  r     r     r  |  r     -  |  r     -  |  o     o     o  |
#define C_CHASE_FADE        23  //same as above, with fading trail                      |  r     r     r  |  r     -  |  r     -  |  o     o     o  |
#define C_CHASE_FADE_B      24  //same as above, with fading trail (backwards)          |  r     r     r  |  r     -  |  r     -  |  o     o     o  |
#define C_CHASE_SD          25  //C_CHASE with random start delay                       |  r     r     r  |  r     -  |  r     -  |  o     o     o  |
#define C_CHASE_B_SD        26  //C_CHASE_B with random start delay                     |  r     r     r  |  r     -  |  r     -  |  o     o     o  |
#define C_CHASE_FADE_SD     27  //C_CHASE_FADE with random start delay                  |  r     r     r  |  r     -  |  r     -  |  o     o     o  |
#define C_CHASE_FADE_B_SD   28  //C_CHASE_FADE_B with random start delay                |  r     r     r  |  r     -  |  r     -  |  o     o     o  |
#define C_SET_ONLY          29  //sets LEDs for matching IDs, turns other modules off   |  r     r     r  |  r     -  |  -     -  |  r     r     r  |
#define C_KNIGHTRIDER       30  //lights chasing back and forth                         |  r     r     r  |  r     -  |  r     -  |  o     o     o  |
#define C_KNIGHTRIDER_SD    31  //same as above, with random start delay                |  r     r     r  |  r     -  |  r     -  |  o     o     o  |
#define C_LIGHTNING         32  //flashes all modules, then returns to its prev. command|  -     -     -  |  -     -  |  -     -  |  o     o     o  |
#define C_SWEEP             33  //sweep along all modules from ID 0 to ID X             |  r     r     r  |  r     -  |  r     -  |  o     o     o  |

//##############################################################################################
//# Parameters:
//##############################################################################################

//   name                   description                                           value             translates to
int  in_command     = 99; //current command                                       0 - 99            command no. 0 - 99                      1
int  in_red         = 0;  //intensity for red                                     0 - 99            0% - 100% red                           2
int  in_green       = 0;  //intensity for green                                   0 - 99            0% - 100% green                         3  
int  in_blue        = 0;  //intensity for blue                                    0 - 99            0% - 100% blue                          4
int  in_bright_max  = 0;  //max. brightness                                       0 - 99            0% - 100% brightness                    5
int  in_bright_min  = 0;  //min. brightness                                       0 - 99            0% - 100% brightness                    6
int  in_time_on     = 0;  //cycle time /cycle on time                             0 - 99            0ms - 9900ms (100 ms steps)             7
int  in_time_off    = 0;  //cycle off time                                        0 - 99            0ms - 9900ms (100 ms steps)             8
int  in_fromID      = 0;  //only apply to IDs >= ..                               0 - 99            apply to IDs starting at 0 - 99         9
int  in_toID        = 99; //only apply to IDs <= ..                               0 - 99            apply to IDs ending at 0 - 99           10
int  in_patternID   = 0;  //only apply to IDs matching the given pattern          0 - 99            apply to pattern no. 0 - 99             11

String inputString;      //String recieved via LoRa
String lastString;       //Backup of last inputString to identify new Data

byte command         = 0;
int  val_red         = 0; int  val_green       = 0; int  val_blue        = 0;
int  val_bright_max  = 0; int  val_bright_min  = 0; int  val_bright      = 0; int  val_bright_span = 0;
int  val_time_on     = 0; int  val_time_off    = 0; int  val_Interval    = 0;
int  val_fromID      = 0; int  val_toID        = 0; int  val_patternID   = 0;

int  last_command    = 0;
int  last_red        = 0; int  last_green      = 0; int  last_blue       = 0;
int  last_bright     = 0;

//##############################################################################################
//# System Variables:
//##############################################################################################

unsigned long currentMillis   =   0;      //current time since starting the arduino in ms
unsigned long previousMillis  =   0;      //timestamp of the last event
unsigned long C_Timestamp     =   0;      //timestamp of the last executed command

byte LEDPointer               =   0;      //keeps track of the current position on the LED strip
bool LEDState                 =   LOW;    //current LED state variable (every LED is the same)
bool StripState[LEDCount];                //current LED state variable (every LED individually)
int  R_Interval               =   1000;   //random interval
byte FadeCounter              =   0;      //keeps track of the fades current position
byte ColorCounter             =   0;      //keeps track of the colorwheels current position
byte ID                       =   0;      //the modules ID

//##############################################################################################
//# Pattern:
//##############################################################################################

#define PatternCount 28                   //number of predefined patterns

bool pattern[PatternCount][ModuleCount] = {                     
//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21  //ID
{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 }, //pattern  1: every odd ID
{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 }, //pattern  2: every even ID
{ 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 }, //pattern  3: every 3rd ID
{ 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0 }, //pattern  4: every 3rd ID NOT
{ 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0 ,0 },  //pattern  5: every 4th ID
{ 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1 ,1 },  //pattern  6: every 4th ID NOT
{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 }, //pattern  7: every 5th ID
{ 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1 }, //pattern  8: every 5th ID NOT
{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }, //pattern  9: every 6th ID
{ 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 }, //pattern 10: every 6th ID NOT
{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0 }, //pattern 11: every 7th ID
{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1 }, //pattern 12: every 7th ID NOT
{ 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }, //pattern 13: every 8th ID
{ 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 }, //pattern 14: every 8th ID NOT
{ 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1 }, //pattern 15: alternating pairs of 2, starting with ID 0
{ 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0 }, //pattern 16: alternating pairs of 2, starting with ID 2
{ 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0 }, //pattern 17: alternating pairs of 3, starting with ID 0
{ 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1 }, //pattern 18: alternating pairs of 3, starting with ID 3
{ 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0 }, //pattern 19: alternating pairs of 4, starting with ID 0
{ 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1 }, //pattern 20: alternating pairs of 4, starting with ID 4
{ 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1 }, //pattern 21: alternating pairs of 5, starting with ID 0
{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0 }, //pattern 22: alternating pairs of 5, starting with ID 5
{ 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 }, //pattern 23: alternating pairs of 6, starting with ID 0
{ 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 }, //pattern 24: alternating pairs of 6, starting with ID 6
{ 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0 }, //pattern 25: alternating pairs of 7, starting with ID 0
{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 }, //pattern 26: alternating pairs of 7, starting with ID 7
{ 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 }, //pattern 27: alternating pairs of 8, starting with ID 0
{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 }  //pattern 28: alternating pairs of 8, starting with ID 8
};

//{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, //pattern XX: all 1
//{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //pattern XX: all 0

//##############################################################################################
//# Setup:
//##############################################################################################

void setup() {
  Serial.begin(9600);
  strip.begin();
  ID = getID();
  if (DemoMode == 1){DemoDone = 0;}
  
  Serial.println("############################################");
  Serial.println("#    LED party lights - Receiver module    #");
  Serial.println("############################################");
  
  Serial.print("[module ID] "); Serial.println(ID);
  
  while (!Serial);

  if (!LoRa.begin(433E6)){ Serial.println("[Receiver] error"); LED_error(); }
  else { LED_success(); Serial.println("[Receiver] waiting for incoming data..."); }
  

}

//##############################################################################################
//# Loop:
//##############################################################################################

void loop() {
  currentMillis = millis();
  int packetSize = LoRa.parsePacket();
  //if (packetSize >0){Serial.println(packetSize);}
  if (packetSize == 22 || !DemoDone) { //Serial.println(packetSize);
    inputString = "";
    while (LoRa.available()) {
      char inChar = (char)LoRa.read();
      inputString += inChar; //inputString.trim();
    }
    if (newString() || !DemoDone){ 
      if (DemoMode) {inputString = DemoString; DemoDone = 1;}
      parseValues();
      printValues();

      if (in_command == C_KILL){ 
        command = C_KILL; 
      }
      else if (in_command == C_SET_ONLY){
        mapValues();
        command = C_SET_ONLY;
      }
      else if (in_command == C_LIGHTNING){
        if (command != C_NULL){
          last_command = command;
        }
        command = C_LIGHTNING;
      }
      else if (checkID() && checkCooldown()){
        mapValues();
        if (in_command == C_PULSE_SD){
          val_bright = random(val_bright_min, val_bright_max);
          if (random(0,1)){LEDState = HIGH;}
          else {LEDState == LOW;}
        }
        Serial.print("[Execution] command "); Serial.println(command);
      } 
    }
  }
  
  if (command != C_NULL){
    switch(command){
      case C_KILL: //98
        strip.clear(); strip.show();
      break;
      //#####################################################################################################################################################
      case C_SET: //1
        val_bright = val_bright_max;
        setStrip(val_red, val_green, val_blue, val_bright);
        last_command = command;
        command = C_NULL;
        
        
      break;
      //#####################################################################################################################################################
      case C_OFF: //2
        strip.clear(); strip.show();
        last_command = command;
        command = C_NULL;
      break;
      //#####################################################################################################################################################
      case C_BLINK: //3
        if (currentMillis - previousMillis >= val_time_off && LEDState == LOW) {
          previousMillis = currentMillis;
          val_bright = val_bright_max;
          setStrip(val_red, val_green, val_blue, val_bright); LEDState = HIGH;
        }
        if (currentMillis - previousMillis >= val_time_on && LEDState == HIGH) {
          previousMillis = currentMillis;
          strip.clear(); strip.show(); LEDState = LOW;
        }  
      break;
      //#####################################################################################################################################################
      case C_BLINK_SD: //4
        previousMillis = currentMillis + random(0, val_Interval);
        command = C_BLINK;
      break;
      //#####################################################################################################################################################
      case C_PULSE: //5
        if ((currentMillis - previousMillis >= val_time_on / val_bright_span/resolution) && LEDState == LOW) {
          val_bright += resolution;
          previousMillis = currentMillis;
        }
        if ((currentMillis - previousMillis >= val_time_off / val_bright_span/resolution) && LEDState == HIGH) {
          val_bright -= resolution;
          previousMillis = currentMillis;
        }
        if (val_bright >= val_bright_max){LEDState = HIGH;}
        if (val_bright <= val_bright_min){LEDState = LOW;}
        setStrip(val_red, val_green, val_blue, val_bright);
      break;
      //#####################################################################################################################################################
      case C_PULSE_SD:  //6 

        if ((currentMillis - previousMillis >= val_time_on / val_bright_span / resolution) && LEDState == LOW) {
          val_bright += resolution;
          previousMillis = currentMillis;
        }
        if ((currentMillis - previousMillis >= val_time_off / val_bright_span / resolution) && LEDState == HIGH) {
          val_bright -= resolution;
          previousMillis = currentMillis;
        }
        if (val_bright >= val_bright_max){LEDState = HIGH;}
        if (val_bright <= val_bright_min){
          LEDState = LOW;
          val_time_on = random(1,3)*in_time_on * random(1, 100);
          val_time_off = random (1,3)*in_time_off * random(1, 100);
        }

        //val_bright_max = random(val_bright_max, val_bright_min);
        //val_bright_min = random(val_bright_max,val_bright_min);

        
        //Serial.println (val_time_on,'\n');
        //Serial.println (  val_time_off,'\n');
        Serial.print ("On:");
        Serial.print (val_time_on);
        Serial.print (" Off:");
        Serial.print (val_time_off);
        Serial.print (" Aktuell:");
        Serial.println (val_bright);
        
        setStrip(val_red, val_green, val_blue, val_bright);

        
      break;
      //#####################################################################################################################################################
      case C_FLASH: //7
        if (currentMillis - previousMillis >= val_time_off && LEDState == LOW) {
          previousMillis = currentMillis;
          val_bright = val_bright_max; setStrip(val_red, val_green, val_blue, val_bright); LEDState = HIGH;
        }
        if (currentMillis - previousMillis >= T_Flash && LEDState == HIGH) {
          previousMillis = currentMillis;
          strip.clear(); strip.show(); LEDState = LOW;
        }  
      break;
      //#####################################################################################################################################################
      case C_FLASH_RT: //8
        if (currentMillis - previousMillis >= R_Interval && LEDState == LOW) {
          previousMillis = currentMillis;
          val_bright = val_bright_max; setStrip(val_red, val_green, val_blue, val_bright); LEDState = HIGH;
          R_Interval = random(val_Interval / 10, val_Interval);
        }
        if (currentMillis - previousMillis >= T_Flash && LEDState == HIGH) {
          strip.clear(); strip.show(); LEDState = LOW;
        }  
      break;
      //#####################################################################################################################################################
      case C_SET_RC: //9
        randomColor(); 
        setStrip(val_red, val_green, val_blue, val_bright);
        last_command = command;
        command = C_NULL;
      break;
      //#####################################################################################################################################################
      case C_FADE: //10
        if ((currentMillis - previousMillis >= val_time_on / 25)) {
          FadeCounter ++;
          previousMillis = currentMillis;
          setStrip( map(FadeCounter, 0, 100, last_red, val_red),
                    map(FadeCounter, 0, 100, last_green, val_green),
                    map(FadeCounter, 0, 100, last_blue, val_blue),
                    map(FadeCounter, 0, 100, last_bright, val_bright_max));
          if (FadeCounter >= 100) {
            last_command = command;
            command = C_NULL;
          }
        }
      break;
      //#####################################################################################################################################################
      case C_PULSE_RCT: //11
        if ((currentMillis - previousMillis >= R_Interval / val_bright_span/resolution) && LEDState == LOW) {
          val_bright += resolution;
          previousMillis = currentMillis;
        }
        if ((currentMillis - previousMillis >= R_Interval / val_bright_span/resolution) && LEDState == HIGH) {
          val_bright -= resolution;
          previousMillis = currentMillis;
        }
        if (val_bright >= val_bright_max){LEDState = HIGH;}
        if (val_bright <= val_bright_min){LEDState = LOW; 
          R_Interval = random(val_time_off, val_Interval);
          randomColor();
        }
        setStrip(val_red, val_green, val_blue, val_bright);
      break;
      //#####################################################################################################################################################
      case C_FLAME: //12
        if (currentMillis - previousMillis >= val_time_on/10) {
          previousMillis = currentMillis;
          val_red = random(128, 255);
          val_green = random (30, 100);
          val_blue = random (0, 10);
          val_bright = random(val_bright_max * 80 / 100, val_bright_max);
          setStrip(val_red, val_green, val_blue, val_bright);
        }
      break;
      //#####################################################################################################################################################
      case C_RAINBOW: //13
        if (currentMillis - previousMillis >= val_time_on / 25) {
          previousMillis = currentMillis;
          wheelColor(ColorCounter);
          setStrip(val_red, val_green, val_blue, val_bright_max);
          ColorCounter ++;
          if (ColorCounter >=256) {ColorCounter = 0;}
        }
      break;
      //#####################################################################################################################################################
      case C_RAINBOW_RC: //14
        ColorCounter = random(1, 255);
        command = C_RAINBOW;
      break;
      //#####################################################################################################################################################
      case C_SHIFT: //15
        if (currentMillis - previousMillis >= val_time_on / LEDCount) {
          previousMillis = currentMillis;
          strip.setPixelColor(FadeCounter -2, strip.Color(val_red, val_green, val_blue));
          strip.setBrightness(map(FadeCounter, 0, 100, last_bright, val_bright_max));
          strip.show();
          if (FadeCounter >= LEDCount) {
            last_command = command;
            command = C_NULL;
          }
          FadeCounter ++;
        }
      break;
      //#####################################################################################################################################################
      case C_SHIFT_B: //16
        if (currentMillis - previousMillis >= val_time_on / LEDCount) {
          previousMillis = currentMillis;
          strip.setPixelColor(LEDCount - FadeCounter, strip.Color(val_red, val_green, val_blue));
          strip.setBrightness(map(FadeCounter, 0, 100, last_bright, val_bright_max));
          strip.show();
          if (FadeCounter >= LEDCount) {
            last_command = command;
            command = C_NULL;
          }
          FadeCounter ++;
        }
      break;
      //#####################################################################################################################################################
      case C_SHIFT_RAINBOW: //17
        if (currentMillis - previousMillis >= val_time_on / 25) {
          previousMillis = currentMillis;
          for (int i = 0; i<=LEDCount; i++) {
            wheelColor(ColorCounter + i * 10);
            strip.setPixelColor(i, strip.Color(val_red, val_green, val_blue));
          }
          strip.setBrightness(val_bright_max); strip.show();
          ColorCounter ++;
          if (ColorCounter >=256) {ColorCounter = 0;}
        }
      break;
      //#####################################################################################################################################################
      case C_SHIFT_RAINBOW_RC: //18
        ColorCounter = random(0, 255);
        command = C_SHIFT_RAINBOW;
      break; 
      //#####################################################################################################################################################
      case C_SHIFT_CHAOTIC: //19
        if (currentMillis - previousMillis >= val_time_on / LEDCount) {
          previousMillis = currentMillis;
          while (1) {
            int i = random (0, LEDCount);
            if (StripState[i] == 0){
              StripState[i] = 1;
              strip.setPixelColor(i, strip.Color(val_red, val_green, val_blue));
              break;
            }
          }
          strip.setBrightness(val_bright_max); strip.show();
          ColorCounter ++;
          if (ColorCounter > LEDCount) {last_command = command; command = C_NULL;}
        }
      break;
      //#####################################################################################################################################################
      case C_SET_RL_RC: //20
        if (currentMillis - previousMillis >= val_time_on / 10) {
          previousMillis = currentMillis;
          randomColor();
          strip.setPixelColor(random(0, LEDCount), strip.Color(val_red, val_green, val_blue));
          strip.setBrightness(val_bright_max); strip.show();
        }
      break;
      //#####################################################################################################################################################
      case C_CHASE: //21
        if (currentMillis - previousMillis >= (val_time_on / (LEDCount - trail))) {
          previousMillis = currentMillis;
          if (LEDPointer > LEDCount + trail) {LEDPointer = 0;}
          strip.clear();
          for (int i = 0; i <= trail; i++){
            strip.setPixelColor(LEDPointer - i, strip.Color(val_red, val_green, val_blue));
          }
          strip.setBrightness(val_bright_max); strip.show();
          LEDPointer ++;
        }
      break;
      //#####################################################################################################################################################
      case C_CHASE_B: //22
        if (currentMillis - previousMillis >= (val_time_on / (LEDCount - trail))) {
          previousMillis = currentMillis;
          if (LEDPointer == 0) {LEDPointer = LEDCount + trail;}
          strip.clear();
          for (int i = 0; i <= trail; i++){
            strip.setPixelColor(LEDPointer + i - trail, strip.Color(val_red, val_green, val_blue));
          }
          strip.setBrightness(val_bright_max); strip.show();
          LEDPointer --;
        }
      break;
      //#####################################################################################################################################################
      case C_CHASE_FADE: //23
        if (currentMillis - previousMillis >= (val_time_on / (LEDCount - trail))) {
          previousMillis = currentMillis;
          if (LEDPointer > LEDCount + trail + 1) {LEDPointer = 0;}
          strip.clear();
          for (int i = 1; i <= trail; i++){
            strip.setPixelColor(LEDPointer - i, strip.Color(val_red / (i*i), val_green / (i*i), val_blue / (i*i)));
          }
          strip.setBrightness(val_bright_max); strip.show();
          LEDPointer ++;
        }
      break;
      //#####################################################################################################################################################
      case C_CHASE_FADE_B: //24
        if (currentMillis - previousMillis >= (val_time_on / (LEDCount - trail))) {
          previousMillis = currentMillis;
          if (LEDPointer == 0) {LEDPointer = LEDCount + trail;}
          strip.clear();
          for (int i = 1; i <= trail; i++){
            strip.setPixelColor(LEDPointer + i - trail, strip.Color(val_red / (i*i), val_green / (i*i), val_blue / (i*i)));
          }
          strip.setBrightness(val_bright_max); strip.show();
          LEDPointer --;
        }
      break;
      //#####################################################################################################################################################
      case C_CHASE_SD: //25
        previousMillis = currentMillis + random(0, val_Interval);
        command = C_CHASE;
      break; 
      //#####################################################################################################################################################
      case C_CHASE_B_SD: //26
        previousMillis = currentMillis + random(0, val_Interval);
        command = C_CHASE_B;
      break; 
      //#####################################################################################################################################################
      case C_CHASE_FADE_SD: //27
        previousMillis = currentMillis + random(0, val_Interval);
        command = C_CHASE_FADE;
      break; 
      //#####################################################################################################################################################
      case C_CHASE_FADE_B_SD: //28
        previousMillis = currentMillis + random(0, val_Interval);
        command = C_CHASE_FADE_B;
      break; 
      //#####################################################################################################################################################
      case C_SET_ONLY: //29
        if (checkID()){command = C_SET;}
        else {command = C_OFF;}
      break; 
      //#####################################################################################################################################################
      case C_KNIGHTRIDER: //30
        if (currentMillis - previousMillis >= (val_time_on / (LEDCount - trail))) {
          previousMillis = currentMillis;
          strip.clear();
          for (int i = 1; i <= trail; i++){
            strip.setPixelColor(LEDPointer + i - 1, strip.Color(val_red, val_green, val_blue));
          }
          strip.setBrightness(val_bright_max); strip.show();
          if (LEDState == LOW) { LEDPointer ++; }
          if (LEDState == HIGH){ LEDPointer --; }
          if (LEDPointer >= LEDCount - trail) {LEDState = HIGH;}
          if (LEDPointer <= 0) {LEDState = LOW;}
        }
      break; 
      //#####################################################################################################################################################
      case C_KNIGHTRIDER_SD: //31
        previousMillis = currentMillis + random(0, val_Interval);
        command = C_KNIGHTRIDER;
      break;
      //#####################################################################################################################################################
      case C_LIGHTNING: //32
        Serial.println("[Execution] command 32 (Lightning)");
        previousMillis = currentMillis + T_Flash;
        setStrip(255, 255, 255, 255);
        delay(T_Flash);
        strip.clear(); strip.show();
        command = last_command;
        Serial.print("[Execution] command ");
        Serial.println(command);
        
      break;
      //#####################################################################################################################################################
      case C_SWEEP: //33
        if (currentMillis - previousMillis >= C_Timestamp + (ID * val_time_on / 25) && LEDState == LOW) {
          previousMillis = currentMillis;
          LEDState = HIGH;
          setStrip(val_red, val_green, val_blue, val_bright_max);
        }
        if (currentMillis - previousMillis >= C_Timestamp + ((ID + 1) * val_time_on / 25) && LEDState == HIGH) {
          previousMillis = currentMillis;
          LEDState = LOW;
          strip.clear(); strip.show();
          command = C_NULL;
        }
      break;
    }
  }
}

void randomColor(){     //generates a random color
  val_red = map(random(0, 4), 0, 3, 0, 255); 
  val_blue = map(random(0, 4), 0, 3, 0, 255); 
  val_green = map(random(0, 4), 0, 3, 0, 255);
}

void wheelColor(byte WheelPos) {  // Input a value 0 to 255 to get a color value. The colours transition from r - g - b - back to r.
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    val_red     = 255 - WheelPos * 3;
    val_green   = 0;
    val_blue    = WheelPos * 3;
  }
  else if(WheelPos < 170) {
    WheelPos -= 85;
    val_red     = 0;
    val_green   = WheelPos * 3;
    val_blue    = 255 - WheelPos * 3;
  }
  else {
    WheelPos -= 170;
    val_red     = WheelPos * 3;
    val_green   = 255 - WheelPos * 3;
    val_blue    = 0;
  }
}

void parseValues(){     //reads all the values from the inputString
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

  val_fromID      = in_fromID;
  val_toID        = in_toID;
  val_patternID   = in_patternID;
}

void mapValues(){       //moves and maps the in_ values to the val_ values
  
  command = in_command;
  C_Timestamp = currentMillis;
 
  last_red = val_red;
  last_green = val_green;
  last_blue = val_blue;
  last_bright = val_bright;
  FadeCounter = 0;
  ColorCounter = 0;
  LEDState = LOW;
  
  if (command != C_PULSE_RCT){
    val_red         = map(in_red, 0, 99, 0, 255);
    val_green       = map(in_green, 0, 99, 0, 255); 
    val_blue        = map(in_blue, 0, 99, 0, 255);
  }
  else {
    randomColor();
  }
  val_bright_max  = map(in_bright_max, 0, 99, 0, 100);
  val_bright_min  = map(in_bright_min, 0, 99, 0, 100);
  val_bright_span = val_bright_max - val_bright_min;

  val_time_on     = constrain(in_time_on * 100, 1, 10000);
  val_time_off    = constrain(in_time_off * 100, 1, 10000);
  val_Interval    = val_time_on + val_time_off;

}

void setStrip(byte red, byte green, byte blue, byte bright){  //sets all LEDs to the given values
  strip.clear(); strip.fill(strip.Color(red, green, blue)); strip.setBrightness(bright); strip.show();
}

byte getID(){           //returns the modules ID set at its ID pins
  byte id = 0;
  pinMode(pin_bit_1, INPUT_PULLUP); if (digitalRead(pin_bit_1)) {id += 1;} 
  pinMode(pin_bit_2, INPUT_PULLUP); if (digitalRead(pin_bit_2)) {id += 2;} 
  pinMode(pin_bit_3, INPUT_PULLUP); if (digitalRead(pin_bit_3)) {id += 4;} 
  pinMode(pin_bit_4, INPUT_PULLUP); if (digitalRead(pin_bit_4)) {id += 8;} 
  pinMode(pin_bit_5, INPUT_PULLUP); if (digitalRead(pin_bit_5)) {id += 16;}
  id = constrain(id, 0, 99);
  return id;
}

bool checkID(){         //checks if the command should be executed by this module
  Serial.print("[ID check] ");
  Serial.print("ID: ");            Serial.print(ID);
  Serial.print(", range from ");   Serial.print(val_fromID);
  Serial.print(" to ");            Serial.print(val_toID);
  Serial.print(", pattern: ");     Serial.print(val_patternID);
  Serial.print(". ");
  if (val_patternID > 0 && val_patternID < PatternCount) { 
    if (pattern[val_patternID][ID] == 1) {Serial.println("Pattern: match"); return true;}
    else {Serial.println("Pattern: no match"); return false;}
  }
  else {
    if (ID >= val_fromID && ID <= val_toID) {Serial.println("Range: match"); return true;} //ID >= val_fromID && ID <= val_toID
    else {Serial.println("Range: no match"); return false;}
  }
}

bool checkCooldown(){   //checks if the in_command requires a cooldown time and if that time has already passed
  Serial.print("[Cooldown] ");
  if (in_command == C_SET_RC          || 
      in_command == C_FADE            ||
      in_command == C_PULSE_RCT       ||
      in_command == C_SHIFT           ||
      in_command == C_SHIFT_B         ||
      in_command == C_SHIFT_CHAOTIC   ||
      in_command == C_SET_ONLY        ||
      in_command == C_LIGHTNING       ||
      in_command == C_SWEEP           )
  {
    if (currentMillis - C_Timestamp >= C_Cooldown)  { Serial.println("passed"); return true; }
    else { 
      if (DemoMode == 1){ Serial.println("Demo Mode"); return true; }
      else { Serial.println("not passed") ;return false; }
    }
  }
  else { Serial.println("not relevant"); return true; }
}

void LED_success(){     //startup sequence on power up
  setStrip(0, 100, 0, 100);
  delay(500); 
  strip.clear(); 
  strip.show();
}
void LED_error(){       //error state when failing to initialize
  while (1){
    setStrip(100, 0, 0, 100);
    delay(200);
    strip.clear();
    strip.show();
    delay(200);
  }
}
bool newString(){       //compares the recieved string to the last one to prevent double triggers
  if (currentMillis - C_Timestamp >= C_Cooldown){ lastString = ""; }
  if (lastString != inputString) { lastString = inputString; return true; }
  else { return false; }
}

void printValues(){     //printing parsed values to the serial monitor
  Serial.println("");
  Serial.print("[Receiver] ");
  Serial.print("Received: ");           Serial.print(inputString); 
  Serial.print(", RSSI: ");             Serial.print(LoRa.packetRssi());
  //Serial.print(", command: ");          Serial.print(in_command); 
  //Serial.print(", red: ");              Serial.print(in_red);
  //Serial.print(", green: ");            Serial.print(in_green);
  //Serial.print(", blue: ");             Serial.print(in_blue);
  //Serial.print(", br. max: ");          Serial.print(in_bright_max);
  //Serial.print(", br. min: ");          Serial.print(in_bright_min);
  //Serial.print(", time on: ");          Serial.print(in_time_on);
  //Serial.print(", time off: ");         Serial.print(in_time_off);
  //Serial.print(", from ID: ");          Serial.print(in_fromID);
  //Serial.print(", to ID: ");            Serial.print(in_toID);
  //Serial.print(", pattern: ");          Serial.print(in_patternID);
  //Serial.print(", packet size: ");      Serial.print(packetSize);
  Serial.println("");
}
