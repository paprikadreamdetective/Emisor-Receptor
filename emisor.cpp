#include <Arduino.h>
#include <irmp.hpp>
#include <TinyIR.h>
#include <TinyIRSender.hpp>
#include <TinyIRReceiver.hpp>
#include <IRremote.h>
#include <IRSend.hpp>

IRsend irsend;


#define LED_BUILT PC13
#define SEND_PIN PA6
#define BUTTON_1 PA0    // Correspondiente al boton para enviar la señal
#define BUTTON_2 PA1
bool OnSignal = false;

//IRMP_DATA send_data;

void setup() {
  //irmp_init();
  pinMode(SEND_PIN, OUTPUT);
  pinMode(LED_BUILT, OUTPUT);
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  //irmp_irsnd_LEDFeedback(true); // Enable send signal feedback at LED_BUILTIN
  /*send_data.protocol = IRMP_NEC_PROTOCOL;
  send_data.address = 0x0707;
  send_data.command = 0x3B; // The required inverse of the 8 bit command is added by the send routine.
  send_data.flags = 2; // repeat frame 2 times
  //irmpData.irsnd_init();
  Serial.begin(115200);*/
}
// Funcon main
void loop() {

  if (digitalRead(BUTTON_1)){
    digitalWrite(LED_BUILT, HIGH);
    sendNEC(SEND_PIN, 0, 0x00ff, 32);
    delay(1000);
  }

  if (!digitalRead(BUTTON_2)){
    sendNEC(SEND_PIN, 0, 0x00ff);
    delay(1000);
    sendNEC(SEND_PIN, 0, 0xff00);
    delay(1000);
  }
}
