// CÓDIGO PARA ESP32-WROOM-32

//NRF24L01 PA/LNA Wireless LED Control
//Código Receptor

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(2, 5);   // CE, CSN
const byte address[6] = "12321"; // Canal de comunicación

const int ledVerde = 32; // Pines de los
const int ledRojo = 25;  // led's a encender

// -------- Protocolo de seguridad --------

// ID válido del transmisor
const int ID_AUTORIZADO = 100;

// clave secreta compartida
const int CLAVE_SEGURIDAD = 12345;

// Estructura del paquete de datos
struct PaqueteDatos {
  int id;
  int humedad;  // Variable para los datos del sensor
  int clave;
};

PaqueteDatos datos;

void setup() {

  Serial.begin(115200);

  // Inicializacion de led's
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);

  // Los inica apagados
  digitalWrite(ledRojo, LOW);
  digitalWrite(ledVerde, LOW);


  // Inicializacion de pines del módulo NRF24L01
  SPI.begin(18, 19, 23, 5);

  Serial.println("\nCircuito Receptor");

  //Verificación del modulo NRF24L01, si es que la detecta
  if (!radio.begin()) {
    Serial.println("No se detecta el nRF24L01");
    while (1);
  }

  radio.openReadingPipe(1, address); // Abre el canal y usa la dirección
  radio.setPALevel(RF24_PA_LOW);  // Ajusta el nivel de potencia
  radio.startListening(); // Activa el modo recepción
}

void loop() {
  // Verifica si hay datos del transmisor
  if (radio.available()) {

    // Recibe los datos del sensor
    radio.read(&datos, sizeof(datos));

    // Imprime los datos recibidos
    Serial.print("Humedad recibida: ");
    Serial.println(datos.humedad);

    // Verifica si es el dispositivo
    if (datos.id == ID_AUTORIZADO && datos.clave == CLAVE_SEGURIDAD) {

      Serial.println("Dispositivo AUTORIZADO");

      // Verifica si el suelo esta humedo
      if (datos.humedad > 1000) {

        Serial.println("Suelo seco");

        digitalWrite(ledRojo, HIGH);
        digitalWrite(ledVerde, LOW);

      // O seco
      } else { 

        Serial.println("Suelo humedo");

        digitalWrite(ledRojo, LOW);
        digitalWrite(ledVerde, HIGH);

      }

    // Anuncio de dispositivo no autorizado
    } else {

      Serial.println("Dispositivo NO AUTORIZADO ⚠️");

      // Apaga todo por seguridad
      digitalWrite(ledRojo, LOW);
      digitalWrite(ledVerde, LOW);

    }
  }
}
