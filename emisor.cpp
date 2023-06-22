//#define USE_ONE_TIMER_FOR_IRMP_AND_IRSND
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
//#define FEEDBACK_LED_IS_ACTIVE_LOW
#define OFF_LED 0xabc
#define ON_LED 0xdef

 // Pin donde está conectado el LED infrarrojo

unsigned int analogValue;
namespace global {
  Adafruit_SSD1306* OLED;
  int iE, iR;
  float vE, vR;
  bool stateLed = false;
  bool stateSignal = false;
}

namespace globalSender {
  IRMP_DATA ir_send;
  IRMP_DATA irmp_dummy_data;
}

#if defined(SEND_SAMSUNG)
union WordUnion
{
    struct
    {
        uint8_t LowByte;
        uint8_t HighByte;
    } UByte;
    struct
    {
        int8_t LowByte;
        int8_t HighByte;
    } Byte;
    uint8_t UBytes[2];
    int8_t Bytes[2];
    uint16_t UWord;
    int16_t Word;
    uint8_t *BytePointer;
};
#endif

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
  #if defined(SEND_SAMSUNG)
    /*
     * Send Samsung32
     */
    irsnd_data.protocol = IRMP_SAMSUNG32_PROTOCOL;
    irsnd_data.address = 0x0707;
    irsnd_data.command = 0xFB04; // For my Samsung, the high byte is the inverse of the low byte
    irsnd_data.flags = 0; // repeat frame 0 time
  #else
    /*
     * Send NEC
     */
    globalSender::ir_send.protocol = IRMP_NEC_PROTOCOL;
    globalSender::ir_send.address = 0x0707;
    globalSender::ir_send.command = 0xFB; // The required inverse of the 8 bit command is added by the send routine.
    globalSender::ir_send.flags = 2; // repeat frame 2 times
  #endif



  if (!irsnd_send_data(&globalSender::ir_send, true)) {
        Serial.println(F("Protocol not found")); // name of protocol is printed by irsnd_data_print()
    }
    irsnd_data_print(&Serial, &globalSender::ir_send);


}

void loop() {


  #if defined(SEND_SAMSUNG)
    // For my Samsung remote, the high byte is the inverse of the low byte
    WordUnion tNextCommand; // using WordUnion saves 14 bytes program memory for the next 3 lines
    tNextCommand.UWord = globalSender::ir_send.command;
    tNextCommand.UByte.HighByte = ~tNextCommand.UByte.LowByte;
    globalSender::ir_send.command = tNextCommand.UWord;
  #endif

  global::OLED->clearDisplay();
  global::OLED->setCursor(0, 0);
  global::OLED->setTextSize(1.5);
  //global::OLED->print("----------\n");       // Lectura analogica del potenciometro.
  analogValue = analogRead(PA1);
  global::vE = analogValue * 1.0;
  global::vE = (global::vE *3.3)/1023.0;

  char buffer[10];
  dtostrf(global::vE, 5, 2, buffer);
  //global::vE = map(global::iE, 0, 1023, 0, 3.3);
  global::OLED->print(buffer);
  global::OLED->print("- send:" + String(global::iE) + "\n");
  global::OLED->print("- Led state: " + String(global::iE) + "\n");
  global::OLED->display();
  global::OLED->print("- " + String(global::vE) + " V\n");
  global::OLED->print("- send:" + String(global::iE) + "\n");
  global::OLED->print("- Led state: " + String(global::iE) + "\n");
  global::OLED->display();
  //delay(1000);

  if(digitalRead(PA2)) // lectura del boton de envio para enviar la señal
    global::stateSignal = !global::stateSignal;
  if (global::stateSignal){
    //globalSender::ir_send.sendRC5(global::iE, 14);
    //globalSender::ir_send.sendSony(0x07b, global::iE, 32);
    globalSender::ir_send.command = global::iE;
    irsnd_send_data(&globalSender::ir_send, true);
  }
    /*global::OLED->print("- send ON:" + String(global::iE) + "\n");
    global::OLED->print("\n");
    global::OLED->display();*/
    //delay(1000);
  /**}else{
    global::OLED->print("- send OFF:" + String(global::iE) + "\n");
    global::OLED->print("\n");
    global::OLED->display();
    //delay(1000);
  }*/
  if(digitalRead(PA3))  // lectura del boton de apagado
    global::stateLed = !global::stateLed;
  if(global::stateLed){
    global::iE = 1023;
    //globalSender::ir_send.sendRC5(global::iE, 14);
    globalSender::ir_send.command = global::iE;
    globalSender::ir_send.address = ON_LED;
    irsnd_send_data(&globalSender::ir_send, true);
    //globalSender::ir_send.sendSony(ON_LED, global::iE, 32);
    /*global::OLED->print("- Led ON: " + String(global::iE) + "\n");
    global::OLED->display();*/
    //global::stateLed = false;
    //delay(1000);
  }else{
    global::iE = 0;
    //globalSender::ir_send.sendRC5(global::iE, 14);
    //globalSender::ir_send.sendSony(OFF_LED, global::iE, 32);
    globalSender::ir_send.command = global::iE;
    globalSender::ir_send.address = OFF_LED;
    irsnd_send_data(&globalSender::ir_send, true);
    /*global::OLED->print("- Led OFF: " + String(global::iE) + "\n");
    global::OLED->display();*/
    //global::stateLed = true;
    //delay(1000);
  }
  /*global::OLED->print("- send:" + String(global::iE) + "\n");
  global::OLED->print("- Led OFF: " + String(global::iE) + "\n");
  global::OLED->display();*/
 // delay(200);
}

/*
namespace globalSender {
IRMP_DATA irsnd_data;

}
void setup() {
    Serial.begin(115200);
    // Just to know which program is running on my Arduino
    //Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRMP));

    irsnd_init();
    irmp_irsnd_LEDFeedback(true); // Enable send signal feedback at LED_BUILTIN

    //Serial.println(F("Send IR signals at pin " STR(IRSND_OUTPUT_PIN)));
    /*
     * Send NEC
     */
    /*irsnd_data.protocol = IRMP_NEC_PROTOCOL;
    irsnd_data.address = 0x0707;
    irsnd_data.command = 0xFB; // The required inverse of the 8 bit command is added by the send routine.
    irsnd_data.flags = 2; // repeat frame 2 times

    // true = wait for frame and trailing space/gap to end. This stores timer state and restores it after sending.
    if (!irsnd_send_data(&irsnd_data, true)) {
        Serial.println(F("Protocol not found")); // name of protocol is printed by irsnd_data_print()
    }
    irsnd_data_print(&Serial, &irsnd_data);
}*/
/*
void loop() {
    delay(5000);
    irsnd_data.command++;

    irsnd_send_data(&irsnd_data, true);
    irsnd_data_print(&Serial, &irsnd_data);
}*/


