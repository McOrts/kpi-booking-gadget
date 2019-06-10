# Gadget DIY para controlar tu empresa.
Con un claro ejemplo de_Over-engineering_ esta es la guía para la fabricación de un pequeño dispositivo para recibir en tiempo real los indicadores más críticos de nuestro negocio. Y poder interactuar con un backend atendiendo a nuestras peticiones.

![](https://github.com/McOrts/kpi-booking-gadget/blob/master/images/Gadget_KPI_portada.PNG?raw=true)

Trabajo para una empresa de distribución global de alojamientos hoteleros. Lo que significa que tiene una infraestuctura descentralizada para poder resolver las peticiones de los clientes en su propia región geográfica. Esto lo permite un sistema de sincronismos y cachés que tienen la contrapartida de generar algunos errores de valoración y disponibilidad de reservas.

Este y otros indicadores aunque son críticos, su accesibilidad está reducida a la consulta a través de un sistema de monitorización como DataDog. __A fin de poder monitorizar de una forma efectiva estas alertas, he ideado un _gadget_ en forma de tarjeta de identificación que, conectada por wifi, traduce en tiempo real las cifras de KPIs a codigos de color y destellos de luz__.

![](https://github.com/McOrts/kpi-booking-gadget/blob/master/images/Gadget_KPI_Front.JPG?raw=true)

Este proyecto representa un ejemplo de cómo la tecnología open-hardware/software puede incorporarse en una gran organización y del bajo coste con el que se puede prototipar hardware.

## Arquitectura
El componente principal de la arquitectura que he utilizado es un _Broker_ de mensajería MQTT (Mosquitto). Los sensores están conectados a puertos GPIO de una mini-CPU que ejecuta un programa especifico para cada uno. Este programa realiza las siguientes tareas:
- Lee el valor del sensor. 
- Graba el valor en una BBDD relacional.
- Envía el valor a:
	- Una plataforma de _Cloud_ via API Rest.
	- Un mensaje MQTT a un _topic_ especifico.
  
![Arquitectura Gadget KPI](https://github.com/McOrts/kpi-booking-gadget/blob/master/images/Gadget_KPI_arquitectura.png?raw=true)



Las tecnologías y herramientas utilizadas son bien conocidad en el mundo maker para IOT:
* Backend: [Node-RED](https://nodered.org/)
* Broker de colas MQTT: [Eclipse Mosquitto](https://mosquitto.org/)
* Micro-controlador: ESP-8266 formato [WEMOS D1 mini](https://wiki.wemos.cc/products:d1:d1_mini)
* Servidor: [Raspberry Pi Zero Wifi](https://www.raspberrypi.org/products/raspberry-pi-zero/)

## El dispositivo
Está construido bajo los principios del _Do It Yourselft_. Con las herramientas básicas para soldar y para trabajar con plásticos y estos materiales. Es posible que cualquier persona pueda construir este _gadget_ por si mismo.



## Siguientes pasos
Existe una tecnologia y técnica de fabricación de componentes en superficie llamada SMD (Surface Mount Device) que permite la minituarización de los circuitos impresos (PCB). Y en esta linea estoy trabajando para tener unos dispositivos 

## Agradecimientos y referencias
- [MQTT.org](http://mqtt.org)
- IBM Developers [MQTT: Enabling the Internet of Things](https://developer.ibm.com/messaging/2013/04/26/mqtt-enabling-internet-things/)
