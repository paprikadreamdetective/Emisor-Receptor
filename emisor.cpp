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

/**
 * @brief Espacio de nombres correspondiente al estado de los botones, lestura analogica del potenciometro e intensidad de emision.
 * @var: OLED: Apuntador de pantalla OLED.
 * @var: analogValue: Lectura de potenciometro.
 * @var: iE: Intensidad de emision.
 * @var: stateLed: Control del estado del boton que enciende y apaga el led.
 * @var: stateSignal: Control del envio de la señal infrarroja.
 * @var: buffer: arreglo de tamaño 10 que contendra la el msj a emitir en la pantalla oled.
 */
namespace global {
  Adafruit_SSD1306 *OLED;
  unsigned int analogValue;
  int iE;
  float vE;
  bool stateLed = false;
  bool stateSignal = false;
  char buffer[10];
}
/**
 * @brief Espacio de nombres correspondientes a la instancia de los objetos para emitir señales infrarrojas.
 * @var: ir_send: Instancia del objeto que emitira la señal por infrarrojo.
 */
namespace globalSender {
  IRMP_DATA ir_send;
}
/**
 * @brief Configuracion de pines y emison de señales.
 * - Se habilitan los pines a usar que en este caso son los siguientes:
 *    + PA0:  Salida del mensaje a emitir.
 *    + PA1: Entrada analogica del potenciometro.
 *    + PA2: Boton que enciende y apaga el led.
 *    + PA3: Boton que habilita el envio de señales infrarrojas.
 * - Tambien se incluye la configuracion de la pantalla oled, asi como la creacion del objeto y color el texto.
 * - Se incluye la configuracion del protocolo de envio de mensajes NEC, asi como la asignacion de uan direccion y un comando a enviar.
 * - Una condicion para verificar la ecistencia del protocolo a usar.
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
  irsnd_init();
  irmp_irsnd_LEDFeedback(true);
  globalSender::ir_send.protocol = IRMP_NEC_PROTOCOL;
  globalSender::ir_send.address = 0x0707;
  globalSender::ir_send.command = 0xFB;
  globalSender::ir_send.flags = 2;
  if (!irsnd_send_data(&globalSender::ir_send, true))
    Serial.println(F("Protocol not found"));
  irsnd_data_print(&Serial, &globalSender::ir_send);
}
/**
 * @brief Funcion principal del programa.
 * - Mensajes en pantalla oled, lectura analogica del potenciometro, condiciones necesarias para la lectura de los botones.
 */
void loop() {
  global::OLED->clearDisplay();
  global::OLED->setCursor(0, 0);
  global::OLED-> setTextSize(2);
  global::OLED->println("Sender");
  global::analogValue = analogRead(PA1);
  global::iE = global::analogValue;
  global::vE = global::analogValue * 1.0;
  global::vE = (global::vE *3.3)/1023.0;
  global::OLED->setTextSize(1);
  dtostrf(global::vE, 5, 2, global::buffer);
  global::OLED->print(global::buffer);
  global::OLED->print(" V\n");
  global::OLED->print("- send:" + String(global::iE) + "\n");
  if (digitalRead(PA2)) {
    global::stateSignal = !global::stateSignal;
    delay(200);
  }
  if (global::stateSignal) {
    globalSender::ir_send.command = global::analogValue;
    irsnd_send_data(&globalSender::ir_send, true);
  }
  if (digitalRead(PA3)) {
    global::stateLed = !global::stateLed;
    delay(200);
    if (global::stateLed) {
      global::OLED->print("- Led state: On \n");
      delay(1000);
      globalSender::ir_send.command = global::iE;
      globalSender::ir_send.address = 0xabc;
      irsnd_send_data(&globalSender::ir_send, true);
    }else{
      //global::iE = 0;
      global::OLED->print("- Led state: Off \n");
      delay(1000);
      globalSender::ir_send.command = global::iE;
      globalSender::ir_send.address = 0xdef;
      irsnd_send_data(&globalSender::ir_send, true);
    }
  }
  global::OLED->display();
  delay(2);
}




