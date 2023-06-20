#include <Arduino.h>
#include <irmp.hpp>
#include <TinyIR.h>
#include <TinyIRSender.hpp>
#include <TinyIRReceiver.hpp>


#define SEND_PIN PA6

IRMP_DATA send_data;

void setup() {
  irmp_init();
  pinMode(SEND_PIN , OUTPUT);
  //irmp_irsnd_LEDFeedback(true); // Enable send signal feedback at LED_BUILTIN
  send_data.protocol = IRMP_NEC_PROTOCOL;
  send_data.address = 0x0707;
  send_data.command = 0x3B; // The required inverse of the 8 bit command is added by the send routine.
  send_data.flags = 2; // repeat frame 2 times
  //irmpData.irsnd_init();
  Serial.begin(115200);
}

void loop() {
  //delay(5000);
  sendNEC(SEND_PIN, send_data.address, send_data.command, send_data.flags);
  delay(1000);  // Espera un tiempo antes de enviar la siguiente señal
  send_data.command++;
}
