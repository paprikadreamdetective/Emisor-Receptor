/**
 *  @file receptor.cpp
 *
 *
 *  @brief Receptor de señales infrarrojas para controlar la intensidad de un led mediante el uso de un potenciometro.
 *
 */
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

/**
 * @brief Espacio de nombres "globalReceiver".
 * - Este espacio contiene la instancia requerida para recibir la señal infrarroja.
 * @var: ir_receiver: Instancia del objeto a manipular para recibir la señal.
 */
namespace globalReceiver {
  IRMP_DATA ir_receiver;
}
/**
 * @brief Espacio de nombres "global".
 * - Contiene el apuntador de tipo Adafruit_SSD1306 para la pantalla oled y otras variables para calular la intensidad del led.
 * @var: iR: intensidad recibida.
 * @var: vR: Voltaje recibido
 * @var: command: almacena los valores de la señal recibida.
 * @var: buffer: arreglo de tamaño 10 que contendra la el msj a emitir en la pantalla oled.
 */
namespace global {
  Adafruit_SSD1306 *OLED;
  int iR;
  float vR;
  static uint8_t command = 0;
  char buffer[10];
}
/**
 * @brief Configuración inicial de la bluepill.
 * - En esta parte se configura el pin PA6 como una salida analogica.
 * - Se inicializa la recepcion de la señal infrarroja usando irmp_init();
 * - Se define la instancia de la pantalla oled, se establece tamaño y color del texto.
 */
void setup() {
  pinMode(ANALOG_OUTPUT, OUTPUT);
  irmp_init();
  Wire.begin(SDA,SCL);
  global::OLED=new Adafruit_SSD1306(OLED_WIDTH, OLED_HEIGHT);
  global::OLED->begin(SSD1306_SWITCHCAPVCC, 60);
  global::OLED->setTextColor(SSD1306_WHITE);
  if (!global::OLED->begin(SSD1306_SWITCHCAPVCC,60))
    utilities::blinkBreakpoint(100);
  global::OLED->setTextColor(SSD1306_WHITE);
}
/**
 * @brief Funcin pirncipal.
 * - Recibe una señal infrarroja que procesa para poder encender un led con un potenciometro
 */
void loop() {
  global::OLED-> clearDisplay();
  global::OLED->setCursor(0,0);
  global::OLED-> setTextSize(2);
  global::OLED->println("IR Rx");
  global::OLED->setTextSize(1);
  global::OLED-> println("command");
  if (irmp_get_data(&globalReceiver::ir_receiver)) {
    global::command = (globalReceiver::ir_receiver.command > 9) ? globalReceiver::ir_receiver.command : 0;
    int address = globalReceiver::ir_receiver.address;
    if (address == OFF_LED) {
      analogWrite(ANALOG_OUTPUT, 0);
      delay(200);
    }
    if (address == ON_LED) {
      global::iR = global::command;
      global::vR = global::command * 1.0;
      global::vR = (global::vR * 3.3)/1023.0;
      dtostrf(global::command, 5, 2, global::buffer);
      analogWrite(ANALOG_OUTPUT, (global::command *150) / 90); // 4 encendidos
    }
  }
  global::OLED->printf("%s", global::buffer);
  global::OLED->display();
}


