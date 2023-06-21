#include <Arduino.h>
#include <IRremote.h>
#include <IRSend.hpp>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SSD1306_Defs.h"
#include <IRremote.h>

#define SEND_PIN PA0

namespace global {
  Adafruit_SSD1306* OLED;
  int iE, iR;
  float vE, vR;
  bool stateLed = true;
  bool stateSignal = false;
}

namespace globalSender {
  IRsend ir_send(SEND_PIN);
}

void setup() {
  pinMode(PA0, OUTPUT);
  pinMode(PA1, INPUT);
  pinMode(PA2, INPUT);
  pinMode(PA3, INPUT);
  pinMode(PA4, INPUT);
  Wire.begin(SDA, SCL);
  global::OLED = new Adafruit_SSD1306(OLED_WIDTH, OLED_HEIGHT);
  global::OLED->begin(SSD1306_SWITCHCAPVCC, 60);
  global::OLED->setTextColor(SSD1306_WHITE);
}

void loop() {
  global::OLED->clearDisplay();
  global::OLED->setCursor(0, 0);
  global::OLED->setTextSize(1.5);
  //global::OLED->print("----------\n");
  global::iE = analogRead(PA1);       // Lectura analogica del potenciometro.
  global::vE = map(global::iE, 0, 1023, 0, 3.3);
  global::OLED->print("- " + String(global::vE) + " V\n");
  global::OLED->display();
  delay(1000);
  if(digitalRead(PA2)) // lectura del boton de envio para enviar la señal
    global::stateSignal = !global::stateSignal;
  if (global::stateSignal){
    globalSender::ir_send.sendSony(0x07b, global::iE, 16);
    global::OLED->print("- send ON:" + String(global::iE) + "\n");
    global::OLED->print("\n");
    global::OLED->display();
    delay(1000);
  }else{
    global::OLED->print("- send OFF:" + String(global::iE) + "\n");
    global::OLED->print("\n");
    global::OLED->display();
    delay(1000);
  }
  if(digitalRead(PA3)){  // lectura del boton de apagado
    delay(50);
    if(global::stateLed){
      global::iE = 1023;
      globalSender::ir_send.sendSony(0x07b, global::iE, 16);
      global::OLED->print("- Led ON: " + String(global::iE) + "\n");
      global::OLED->display();
      global::stateLed = false;
      delay(1000);
    }else{
      global::iE = 0;
      globalSender::ir_send.sendSony(0x07b, global::iE, 16);
      global::OLED->print("- Led OFF: " + String(global::iE) + "\n");
      global::OLED->display();
      global::stateLed = true;
      delay(1000);
    }
  }
}
