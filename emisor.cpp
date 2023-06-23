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

namespace globalReceiver{
  IRMP_DATA ir_receiver;
}

namespace global {
  Adafruit_SSD1306* OLED;
  int iR;
  float vR;
}
/**
 * @brief Configuración inicial de la bluepill.
 * - En esta parte se configura el pin PA6 como una salida analogica.
 * - Se inicializa la recepcion de la señal infrarroja usando irmp_init();
 * - Se define la instancia de la pantalla oled, se establece tamaño y color del texto.
 */
void setup(){

  Serial.begin(115200);
  pinMode(ANALOG_OUTPUT, OUTPUT);
  irmp_init();
  irmp_print_active_protocols(&Serial);
  Wire.begin(SDA,SCL);
  global::OLED=new Adafruit_SSD1306(OLED_WIDTH, OLED_HEIGHT);
  global::OLED->begin(SSD1306_SWITCHCAPVCC, 60);
  global::OLED->setTextColor(SSD1306_WHITE);
  if(!global::OLED->begin(SSD1306_SWITCHCAPVCC,60))
    utilities::blinkBreakpoint(100);
  global::OLED->setTextColor(SSD1306_WHITE);
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

if(irmp_get_data(&globalReceiver::ir_receiver)){
  if(globalReceiver::ir_receiver.command > 9)
    command = globalReceiver::ir_receiver.command;
  else
    command = 0;

  int address = globalReceiver::ir_receiver.address;

  if (address == OFF_LED){
    analogWrite(ANALOG_OUTPUT, 0);
    delay(200);
  }
  if (address == ON_LED){
    global::iR = command;
    global::vR = command * 1.0;
    global::vR = (global::vR * 3.3)/1023.0;
    //dtostrf(global::vR, 3,1,buffer);
    dtostrf(command, 5,2,buffer);
    //analogWrite (ANALOG_OUTPUT, (command * 1023)/100);
    //analogWrite (ANALOG_OUTPUT, (command * 255)/100);
    analogWrite(ANALOG_OUTPUT, (command*150)/90); // 4 encendidos

  }
}

global::OLED->printf("%s", buffer);
global::OLED->display();

//delay(500);
}

