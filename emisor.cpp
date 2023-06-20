#include <Arduino.h>
#include <irmp.hpp>
#include <TinyIR.h>
#include <TinyIRSender.hpp>
#include <TinyIRReceiver.hpp>

#define RECV_PIN PA6   // Pin de recepción del receptor infrarrojo
#define LED_PIN  PC13  // Pin del LED

IRMP_DATA irmpData;
bool ledState;
bool state;
void setup() {
 pinMode(LED_PIN, OUTPUT);   // Configurar el pin del LED como salida
 pinMode(RECV_PIN, INPUT);
  irmp_init();
  Serial.begin(115200);
  irmpData.protocol = IRMP_NEC_PROTOCOL;
  irmpData.address = 0x0707;
  irmpData.command = 0x3B; // The required inverse of the 8 bit command is added by the send routine.
  irmpData.flags = 2; // repeat frame 2 times*/
  //irmp_add_protocol(&IRMP_NEC_PROTOCOL);  // Agregar el protocolo NEC al receptor
  //irmp_enable();
}

void loop() {
  state = digitalRead(RECV_PIN);
  if (irmp_get_data(&irmpData)) {
    
    if (state)
      digitalWrite(LED_PIN, HIGH);
    else
      digitalWrite(LED_PIN, LOW);



    //irmp_reset_data();  // Reiniciar el buffer de datos para recibir el próximo código IR
  }
    delay(1000);
  
}
