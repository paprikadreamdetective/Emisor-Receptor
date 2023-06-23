/**
 *  @file emisor.cpp
 *
 *
 *  @brief Emisor de señales infrarrojas para controlar la intensidad de un led mediante el uso de un potenciometro.
 *
 */
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SSD1306_Defs.h>

#define IRSND_IR_FREQUENCY 38000
#define IRSND_PROTOCOL_NAMES 1
#define IRSND_SUPPORT_NEC_PROTOCOL 1
#define IRSND_OUTPUT_PIN PA0
#include <irsndSelectMain15Protocols.h>
#include <irsnd.hpp>

#define ON_LED 0xabc
#define OFF_LED 0xdef

/**
 * @brief
 *
 */
namespace global {
  Adafruit_SSD1306* OLED;
  unsigned int analogValue;
  int iE, iR;
  float vE, vR;
  bool stateLed = false;
  bool stateSignal = false;
}
/**
 * @brief
 *
 */
namespace globalSender {
  IRMP_DATA ir_send;
  IRMP_DATA irmp_dummy_data;
}
/**
 * @brief
 *
 */
void setup() {
  pinMode(PA0, OUTPUT);
  pinMode(PA1, INPUT);
  pinMode(PA2, INPUT);
  pinMode(PA3, INPUT);
  Wire.begin(SDA, SCL);
  global::OLED = new Adafruit_SSD1306(OLED_WIDTH, OLED_HEIGHT);
  global::OLED->begin(SSD1306_SWITCHCAPVCC, 60);
  global::OLED->setTextColor(SSD1306_WHITE);

  // Inicializa la biblioteca IRMP
  irsnd_init();
  //irmp_init();
  //irmp_print_active_protocols(&Serial);
  irmp_irsnd_LEDFeedback(true);
    /*
     * Send NEC
     */
    globalSender::ir_send.protocol = IRMP_NEC_PROTOCOL;
    globalSender::ir_send.address = 0x0707;
    globalSender::ir_send.command = 0xFB; // The required inverse of the 8 bit command is added by the send routine.
    globalSender::ir_send.flags = 2; // repeat frame 2 times

  if (!irsnd_send_data(&globalSender::ir_send, true)) {
        Serial.println(F("Protocol not found")); // name of protocol is printed by irsnd_data_print()
    }
    irsnd_data_print(&Serial, &globalSender::ir_send);


}
/**
 * @brief
 *
 */
void loop() {

  global::OLED->clearDisplay();
  global::OLED->setCursor(0, 0);
  global::OLED->setTextSize(1.5);
  //global::OLED->print("----------\n");       // Lectura analogica del potenciometro.
  analogValue = analogRead(PA1);
  global::iE = analogValue;
  global::vE = analogValue * 1.0;
  global::vE = (global::vE *3.3)/1023.0;

  char buffer[10];
  dtostrf(global::vE, 5, 2, buffer);
  //global::vE = map(global::iE, 0, 1023, 0, 3.3);
  global::OLED->print(buffer);
  global::OLED->print("- send:" + String(global::iE) + "\n");
  global::OLED->print("- Led state: " + String(global::iE) + "\n");


  //delay(1000);

  if (digitalRead(PA2)){
    global::stateSignal = !global::stateSignal;
    delay(200);
  } // lectura del boton de envio para enviar la señal
  if (global::stateSignal){
    globalSender::ir_send.command = analogValue;
    irsnd_send_data(&globalSender::ir_send, true);
  }
  /*else{
    globalSender::ir_send.command = global::iE;
    irsnd_send_data(&globalSender::ir_send, true);
  }*/
  if(digitalRead(PA3)){
    global::stateLed = !global::stateLed;
    delay(200);
    // lectura del boton de apagado
    if(global::stateLed){
      //global::iE = 1023;
      //globalSender::ir_send.sendRC5(global::iE, 14);
      global::OLED->print("- Led state: On" + String(global::iE) + "\n");
      globalSender::ir_send.command = global::iE;
      globalSender::ir_send.address = 0xabc;
      irsnd_send_data(&globalSender::ir_send, true);
    }else{
      global::iE = 0;
      global::OLED->print("- Led state: Off" + String(global::iE) + "\n");
      globalSender::ir_send.command = global::iE;
      globalSender::ir_send.address = 0xdef;
      irsnd_send_data(&globalSender::ir_send, true);
    }
  }
  global::OLED->display();
  delay(2);
}






