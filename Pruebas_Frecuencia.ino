//Código para ESP32S3 con Led integrado

//NRF24L01 PA/LNA Wireless LED Control
//Código Transmisor 

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

const int sensorH = 2; // Pin del sensor "soil humidity"

RF24 radio(4, 5);  // CE, CSN
const byte address[6] = "12321"; // Canal de comunicación

// -------- Protocolo de seguridad --------

// ID del dispositivo
const int ID_DISPOSITIVO = 100;

// clave secreta (debe coincidir con el receptor)
const int CLAVE_SEGURIDAD = 12345;

// Estructura del paquete (igual que en el receptor)
struct PaqueteDatos {
  int id;
  int humedad;
  int clave;
};

PaqueteDatos datos;

void setup() {

  Serial.begin(115200);
  Serial.println("\nCircuito Transmisor");

  // Inicializacion de pines del módulo NRF24L01
  SPI.begin(12, 13, 11, 5);  // SCK, MISO, MOSI, SS (CSN)

  // Verificación del modulo NRF24L01, si es que la detecta
  if (!radio.begin()) {
    //Si no la encuentra muestra mensaje y se detiene
    Serial.println("No se detecta el nRF24L01");
    while (1);
  }

  radio.openWritingPipe(address); // Direcciona los mensajes
  radio.setPALevel(RF24_PA_LOW);  // Ajusta el nivel de potencia
  radio.stopListening();  // Desactiva el modo Recepcion y queda como transmisor
}

void loop() {

  // Obtiene los valores del sensor
  int humedad = analogRead(sensorH);

  // Muestra los valores que recibe del sensor 
  Serial.print("Valor humedad: ");
  Serial.println(humedad);

  // ----- Crea el paquete a enviar -----
  datos.id = ID_DISPOSITIVO;
  datos.humedad = humedad;
  datos.clave = CLAVE_SEGURIDAD;

  // Envía los dato del sensor por el módulo
  radio.write(&datos, sizeof(datos));

  // Verifica el envío
  if (ok) {
    Serial.println("Paquete enviado correctamente");
  } else {
    Serial.println("Error al enviar");
  }

  delay(1000);  // Un pequeño tiempo de espera para evitar saturaciones
}
