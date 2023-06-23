#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SSD1306_Defs.h>
#include <utilities.h>
#include <irsndSelectMain15Protocols.h>

#define IRMP_PROTOCOL_NAMES 1
#define IRMP_SUPPORT_NEC_PROTOCOL 1
#define FEEDBACK_LED_IS_ACTIVE_LOW
#define IRMP_INPUT_PIN PA0
#include <irmp.hpp>

#define ON_LED 0xabc
#define OFF_LED 0xdef

#define ANALOG_OUTPUT PA6

IRMP_DATA irmp_data;

namespace global {
  Adafruit_SSD1306* OLED;
  int iE, iR;
  float vE, vR;
  bool stateLed = false;
  bool stateSignal = false;
}

/*namespace global{
    unsigned int analogReading;
    Adafruit_SSD1306 *oled;

}*/

void setup(){

    Serial.begin(115200);
    pinMode(ANALOG_OUTPUT, OUTPUT);
    irmp_init();
    irmp_print_active_protocols(&Serial);


Wire.begin(SDA,SCL);
global::OLED=new Adafruit_SSD1306(OLED_WIDTH, OLED_HEIGHT);
global::OLED->begin(SSD1306_SWITCHCAPVCC, 60);
global::OLED->setTextColor(SSD1306_WHITE);
if(!global::OLED->begin(SSD1306_SWITCHCAPVCC,60)){
utilities::blinkBreakpoint(100);
global::OLED->setTextColor(SSD1306_WHITE);
 }
}

void loop(){
static uint8_t command = 0;

char buffer[10];

global::OLED-> clearDisplay();
global::OLED->setCursor(0,0);
global::OLED-> setTextSize(2);
global::OLED->println("IR Rx");
global::OLED->setTextSize(1);
global::OLED-> println("command");

if(irmp_get_data(&irmp_data)){
  if(irmp_data.command > 9)
    command = irmp_data.command;
  else
    command = 0;

  int address = irmp_data.address;

  if (address == OFF_LED){
    analogWrite(ANALOG_OUTPUT, 1);
    delay(200);
  }
  if (address == ON_LED){
    global::iR = command;
    global::vR = command * 1.0;
    global::vR = (global::vR * 3.3)/1023.0;
    dtostrf(global::vR, 5,2,buffer);
    //analogWrite (ANALOG_OUTPUT, (command * 1023)/100);
    //analogWrite (ANALOG_OUTPUT, (command * 255)/100);
    analogWrite(ANALOG_OUTPUT, (command*150)/90); // 4 encendidos

  }
}

global::OLED->printf("%s", buffer);
global::OLED->display();

//delay(500);
}

