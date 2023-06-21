#include <Arduino.h>
#include <IRremote.h>   // Libreria que proporciona los metodos necesarios para recibir una señal
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SSD1306_Defs.h"
#include <IRremote.h>

#define RECV_PIN A6 // Pin que recibira la señal del emisor infrarrojo (conectado a el receptor)
#define LED A0      // Pin del led a encender

// Contiene los atributos de la pantalla OLED
namespace global {
  Adafruit_SSD1306* OLED;
  int iE, iR;
  float vE, vR;
  bool stateLed = true;
  bool stateSignal = false;
}
// Campos correspondientes a al objeto que recibira la señal
namespace globalReceiver{
  IRrecv irrecv(RECV_PIN);
  decode_results results;
}

void setup(){
  pinMode(LED, OUTPUT);
  globalReceiver::irrecv.enableIRIn();
  Wire.begin(SDA, SCL);
  global::OLED = new Adafruit_SSD1306(OLED_WIDTH, OLED_HEIGHT);
  global::OLED->begin(SSD1306_SWITCHCAPVCC, 60);
  global::OLED->setTextColor(SSD1306_WHITE);
}

void loop(){


    static uint8_t command = 0;
    static float V = 0;
    char buffer[10];

    global::OLED->clearDisplay();
    global::OLED->setCursor(0, 0);
    global::OLED->setTextSize(1.5);
    global::vE = map(globalReceiver::irrecv.decodedIRData.command, 0, 1023, 0, 3.3);
    global::OLED->print("- " + String(global::vE) + " V\n");
    global::OLED->display();
    delay(1000);

    //if (globalReceiver::irrecv.decodedIRData.command)
    if (globalReceiver::irrecv.decode()){
      if (globalReceiver::irrecv.decodedIRData.command > 9) {
        command = globalReceiver::irrecv.decodedIRData.command;
      }
    }
    /*
    digitalWrite(LED, state);*/
    digitalWrite(LED, HIGH);  // Enciende el LED
    delay(1000);                   // Espera 1 segundo
    digitalWrite(LED, LOW);   // Apaga el LED
    globalReceiver::irrecv.resume();
  delay(200);
}


