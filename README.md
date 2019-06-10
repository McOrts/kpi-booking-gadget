# Gadget DIY para controlar tu empresa.
Con un claro ejemplo de_Over-engineering_ esta es la guía para la fabricación de un pequeño dispositivo para recibir en tiempo real los indicadores más críticos de nuestro negocio. Y poder interactuar con un backend atendiendo a nuestras peticiones.

Trabajo para una empresa de distribución global de alojamientos hoteleros. Lo que significa que tiene una infraestuctura descentralizada para poder resolver las peticiones de los clientes en su propia región geográfica. Esto lo permite un sistema de sincronismos y cachés que tienen la contrapartida de generar algunos errores de valoración y disponibilidad de reservas.

Este y otros indicadores son críticos y la accesibilidad está reducida a la consulta a través de un sistema de monitorización como DataDog.

Este proyecto representa un ejemplo de cómo la tecnología open-hardware/software puede incorporarse en una gran organización y de cómo se pude aplicar a

Las tecnologías y herramientas utilizadas son bien conocidad en el mundo maker para IOT:
* Backend: [Node-RED](https://nodered.org/)
* Broker de colas MQTT: [Eclipse Mosquitto](https://mosquitto.org/)
* Micro-controlador: ESP-8266 formato [WEMOS D1 mini](https://wiki.wemos.cc/products:d1:d1_mini)
* Servidor: [Raspberry Pi Zero Wifi](https://www.raspberrypi.org/products/raspberry-pi-zero/)



## Agradecimientos y referencias
- [MQTT.org](http://mqtt.org)
- IBM Developers [MQTT: Enabling the Internet of Things](https://developer.ibm.com/messaging/2013/04/26/mqtt-enabling-internet-things/)
