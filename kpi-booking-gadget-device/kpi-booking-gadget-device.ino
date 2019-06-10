#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "settings.h"

/* Configuración cliente WiFi */
WiFiClient espClient;

/* Configuración MQTT */
PubSubClient clientMqtt(espClient);
long lastMsg = 0;
char msg[50];
String mqttcommand = String(14);

/* Configuración led NEOPIXEL */
int RatioColor = 0;
#define PIN_STRIP_1 2
#define NUMPIXELS_STRIP_1 3
Adafruit_NeoPixel pixels_STRIP_1  = Adafruit_NeoPixel(NUMPIXELS_STRIP_1, PIN_STRIP_1, NEO_GRB + NEO_KHZ800);

/* Configuración botón del pánico */
const int PanicStatePin = 4;
int PanicState = 0;

void setup() {
  Serial.begin(115200);

  /* Iniciar botón del pánico */
  pinMode(PanicStatePin, INPUT);  // input pin for switch
  digitalWrite(PanicStatePin, LOW);

  /* Iniciar NEOPIXEL */
  pixels_STRIP_1.begin(); // This initializes the NeoPixel library. 
  for(int i=0;i<NUMPIXELS_STRIP_1;i++){
     pixels_STRIP_1.setPixelColor(i,0,0,0); // black color.
     pixels_STRIP_1.show(); // This sends the updated pixel color to the hardware.
  }

  /* Iiniciar wifi */
  setup_wifi();
  clientMqtt.setServer(mqtt_server, mqtt_port);
  clientMqtt.setCallback(callback);
}

void setup_wifi() {
  delay(10);

  // Comienza el proceso de conexión a la red WiFi
  Serial.println();
  Serial.print("[WIFI]Conectando a ");
  Serial.println(ssid);

  // Modo estación
  WiFi.mode(WIFI_STA);
  // Inicio WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("[WIFI]WiFi conectada");
  Serial.print("[WIFI]IP: ");
  Serial.print(WiFi.localIP());
  Serial.println("");
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("[MQTT]Mensaje recibido (");
  Serial.print(topic);
  Serial.print(") ");
  mqttcommand = "";
  for (int i = 0; i < length; i++) {
    mqttcommand += (char)payload[i];
  }
  Serial.print(mqttcommand);
  Serial.println();
  // Color de cada NeoPixel según el mensaje MQTT
  if (mqttcommand.substring(0,9) == "bookerror") {
    RatioColor = mqttcommand.substring(9,11).toInt();
    if (RatioColor > 50) {
      RatioColor = 50;
    }
    Serial.println (RatioColor);
    if (RatioColor <= 25 ) {
      RatioColor = RatioColor*10;
      pixels_STRIP_1.setPixelColor(0,RatioColor,255,0);
    } else {
      RatioColor = 255-((RatioColor-25)*10);
      pixels_STRIP_1.setPixelColor(0,255,RatioColor,0);      
    }
    pixels_STRIP_1.setPixelColor(1,0,0,0);
    pixels_STRIP_1.setPixelColor(2,0,0,0);
    pixels_STRIP_1.show(); // This sends the updated pixel color to the hardware.
    Serial.println("Light Booking Error On");
  } 
  else if (mqttcommand == "evolutionDW") {
    for(int i=1;i<255;i++){
      pixels_STRIP_1.setPixelColor(1,255-i,255-i,255-i);
      pixels_STRIP_1.setPixelColor(2,0,0,0);
      pixels_STRIP_1.show(); // This sends the updated pixel color to the hardware.
      delay(update_time/40);
    }
    pixels_STRIP_1.setPixelColor(1,0,0,0); 
    pixels_STRIP_1.setPixelColor(2,0,0,0);
    pixels_STRIP_1.show(); // This sends the updated pixel color to the hardware.
    Serial.println("Evolution Down");
  } 
  else if (mqttcommand == "evolutionUP") {
    for(int i=1;i<255;i++){
      pixels_STRIP_1.setPixelColor(1,i,i,i);
      pixels_STRIP_1.setPixelColor(2,0,0,0);
      pixels_STRIP_1.show(); // This sends the updated pixel color to the hardware.
      delay(update_time/40);
    }
    pixels_STRIP_1.setPixelColor(1,0,0,0); 
    pixels_STRIP_1.setPixelColor(2,0,0,0);
    pixels_STRIP_1.show(); // This sends the updated pixel color to the hardware.
    Serial.println("Evolution Down");
  } 
  else if (mqttcommand == "warnings") {
    for(int i=1;i<20;i++){
      pixels_STRIP_1.setPixelColor(1,0,0,0);
      pixels_STRIP_1.setPixelColor(2,255,0,0);
      pixels_STRIP_1.show(); // This sends the updated pixel color to the hardware.
      delay(update_time/40);
      pixels_STRIP_1.setPixelColor(1,0,0,0);
      pixels_STRIP_1.setPixelColor(2,0,0,255);
      pixels_STRIP_1.show(); // This sends the updated pixel color to the hardware.
      delay(update_time/40);
    }
    pixels_STRIP_1.setPixelColor(1,0,0,0); 
    pixels_STRIP_1.setPixelColor(2,0,0,0);
    pixels_STRIP_1.show(); // This sends the updated pixel color to the hardware.
    Serial.println("Warnings");
  } 
  else if (mqttcommand == "LightOff"){
    for(int i=0;i<NUMPIXELS_STRIP_1;i++){
      pixels_STRIP_1.setPixelColor(i, pixels_STRIP_1.Color(0,0,0)); // black color.
      pixels_STRIP_1.show(); // This sends the updated pixel color to the hardware.
    }
    Serial.println("LightOff");
  } 
  else if (mqttcommand == "LightOn"){
    for(int i=0;i<NUMPIXELS_STRIP_1;i++){
      pixels_STRIP_1.setPixelColor(i, pixels_STRIP_1.Color(100,100,100)); // black color.
      pixels_STRIP_1.show(); // This sends the updated pixel color to the hardware.
    }
    Serial.println("LightOn");
  } 
}

void reconnect() {
  Serial.print("[MQTT]Intentando conectar a servidor MQTT... ");
  // Bucle hasta conseguir conexión
  while (!clientMqtt.connected()) {
    Serial.print(".");
    // Intento de conexión
    if (clientMqtt.connect(mqtt_id)) { // Ojo, para más de un dispositivo cambiar el nombre para evitar conflicto
      Serial.println("");
      Serial.println("[MQTT]Conectado al servidor MQTT");
      // Once connected, publish an announcement...
      clientMqtt.publish(mqtt_sub_topic_healthcheck, "starting");
      // ... and subscribe
      clientMqtt.subscribe(mqtt_sub_topic_operation);
    } else {
      Serial.print("[MQTT]Error, rc=");
      Serial.print(clientMqtt.state());
      Serial.println("[MQTT]Intentando conexión en 5 segundos");
      delay(5000);
    }
  }
}

void loop() {
  if (!clientMqtt.connected()) {
    reconnect();
  }
  clientMqtt.loop();

  //Botón del pánico
  PanicState = digitalRead(PanicStatePin);
  if (PanicState == LOW) {
    Serial.println("Panic pulsado");
    clientMqtt.publish(mqtt_sub_topic_panic, "On");
    delay(update_time);
    clientMqtt.publish(mqtt_sub_topic_panic, "Off"); 
  }
}
