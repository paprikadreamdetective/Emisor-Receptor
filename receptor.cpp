#include <IRMP.h>

const int RECV_PIN = PA0; // Pin de conexión del módulo receptor infrarrojo
const int LED_PIN = PC13; // Pin para el LED

IRMP irrecv(RECV_PIN);
IRMP_DATA results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Iniciar el receptor infrarrojo
  pinMode(LED_PIN, OUTPUT); // Configurar el pin del LED como salida
}

void loop() {
  if (irrecv.available()) {
    // Si se recibió una señal infrarroja, encender el LED
    digitalWrite(LED_PIN, HIGH);
    irrecv.read(&results);
    Serial.print("Valor recibido: 0x");
    Serial.println(results.command, HEX);
  } else {
    // Si no se recibió una señal infrarroja, apagar el LED
    digitalWrite(LED_PIN, LOW);
  }
}


