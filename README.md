# Gadget DIY para controlar tu empresa.
Con un claro ejemplo de_Over-engineering_ esta es la guía para la fabricación de un pequeño dispositivo para recibir en tiempo real los indicadores más críticos de nuestro negocio. Y poder interactuar con un backend atendiendo a nuestras peticiones.

![](https://github.com/McOrts/kpi-booking-gadget/blob/master/images/Gadget_KPI_portada.PNG?raw=true)

Trabajo para una empresa de distribución global de alojamientos hoteleros. Lo que significa que tiene una infraestuctura descentralizada para poder resolver las peticiones de los clientes en su propia región geográfica. Esto lo permite un sistema de sincronismos y cachés que tienen la contrapartida de generar algunos errores de valoración y disponibilidad de reservas.

Este y otros indicadores aunque son críticos, su accesibilidad está reducida a la consulta a través de un sistema de monitorización como DataDog. __A fin de poder monitorizar de una forma efectiva estas alertas, he ideado un _gadget_ en forma de tarjeta de identificación que, conectada por wifi, traduce en tiempo real las cifras de KPIs a codigos de color y destellos de luz__.

![](https://github.com/McOrts/kpi-booking-gadget/blob/master/images/Gadget_KPI_Front.JPG?raw=true)

Este dispositivo que representa el _front_ de la aplicación. Tiene tres indicadores luminosos (NeoPixel) y un pulsador que, de izquierda a derecha, son para:
- Error en las confirmaciones de reserva: 0% verde al 5% rojo.
- Ventas por el canal B2B2C: 
-- Se reducen: de blanco a negro
-- Se incrementan: de negro a blanco
- Alertas planetarias indicadas con destellos azules y rojos:
-- Nuevos terremotos en cualquier parte del mundo de cualquier intensidad en la última hora.
-- Post de un nuevo Twitter de Donald Trump (@realDonaldTrump).

Este proyecto representa un ejemplo de cómo la tecnología open-hardware/software puede incorporarse en una gran organización y del bajo coste con el que se puede prototipar hardware.

## Arquitectura
El componente principal que he utilizado en esta arquitectura es un _Broker_ (Mosquitto) de mensajería tipo MQTT. En el _backend_ hay  un orquestador extremadamente sencillo de programar, Node-RED. Como dispositivos periféricos tenermos la tarjeta con los indicadores LED, el teléfono móvil con la pulsera inteligente asociada y un robot modelo OTTO. Todo interconectado con una red Wifi. 

![Arquitectura Gadget KPI](https://github.com/McOrts/kpi-booking-gadget/blob/master/images/Gadget_KPI_arquitectura.png?raw=true)

La operativa de toda la aplicación parte de las consultas que se hacen desde el _backend_ (Node-RED):
- Se lanza una petición REST la los web-services:
-- DATADOG para los indicadores de negocio. [](https://app.datadoghq.com/api/v1/query?api_key=...)
-- GeoJSON servicio público de alertas globales de terremotos. [](https://earthquake.usgs.gov/earthquakes/feed/v1.0/summary/all_hour.geojson)
- Se componen los mensajes para los topics de MQTT que la tarjeta está escuchando. La estructura es así:
```
`-- hbg
    |-- kpi
    |   `-- panic
    |   `-- operation
```
evolutionDW
bookerror21
warnings

El Estos dos componentes están ejecutándose en el servidor (Raspberry Pi Zero) 


Las tecnologías y herramientas utilizadas son bien conocidad en el mundo maker para IOT:
* Backend: [Node-RED](https://nodered.org/)
* Broker de colas MQTT: [Eclipse Mosquitto](https://mosquitto.org/)
* Micro-controlador: ESP-8266 formato [WEMOS D1 mini](https://wiki.wemos.cc/products:d1:d1_mini)
* Servidor: [Raspberry Pi Zero Wifi](https://www.raspberrypi.org/products/raspberry-pi-zero/)
* Robot: [Otto](https://www.ottodiy.com/) basado en una versión Wifi no oficial del Arduino Nano.

## El dispositivo
Está construido bajo los principios del _Do It Yourselft_. Con las herramientas básicas para soldar y para trabajar con plásticos y estos materiales. Es posible que cualquier persona pueda construir este _gadget_ por si mismo.



## Siguientes pasos
Existe una tecnologia y técnica de fabricación de componentes en superficie llamada SMD (Surface Mount Device) que permite la minituarización de los circuitos impresos (PCB). Y en esta linea estoy trabajando para tener unos dispositivos 

## Agradecimientos y referencias
- [MQTT.org](http://mqtt.org)
- IBM Developers [MQTT: Enabling the Internet of Things](https://developer.ibm.com/messaging/2013/04/26/mqtt-enabling-internet-things/)
