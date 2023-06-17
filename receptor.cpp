/*#include <IRremote.h>

const int RECV_PIN = PA0; // Pin de conexión del módulo receptor infrarrojo
const int LED_PIN = PC13; // Pin para el LED

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Iniciar el receptor infrarrojo
  pinMode(LED_PIN, OUTPUT); // Configurar el pin del LED como salida
}

void loop() {
  if (irrecv.decode(&results)) {
    // Si se recibió una señal infrarroja, encender el LED
    digitalWrite(LED_PIN, HIGH);
    Serial.print("Valor recibido: 0x");
    Serial.println(results.value, HEX);
    irrecv.resume(); // Recibir el siguiente valor
  } else {
    // Si no se recibió una señal infrarroja, apagar el LED
    digitalWrite(LED_PIN, LOW);
  }
}
*/



#include <IRremote.h>
#include <IRremoteInt.h>

const uint8_t pin_receptor = A0; // Pin del receptor IR
IRrecv irrecv(pin_receptor);
decode_results resultado;

void setup() {
  Serial.begin(9600);
  pinMode(A0,INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  irrecv.enableIRIn();
  Serial.println("Receptor inicializado");
}

void loop() {
  if (irrecv.decode() ){
    //Serial.print("Código recibido: ");
   // Serial.println(resultado.value, HEX);
    irrecv.resume();
  }
  delay(1);
}

