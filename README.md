# Gadget DIY para controlar tu empresa.
Con un claro ejemplo de _Over-engineering_ esta es la guía para la fabricación de un pequeño dispositivo que recibirá en tiempo real los indicadores más críticos de nuestro negocio. Y poder interactuar con un backend atendiendo a nuestras peticiones.

![](https://github.com/McOrts/kpi-booking-gadget/blob/master/images/Gadget_KPI_portada.PNG?raw=true)

Trabajo para una empresa de distribución global de alojamientos hoteleros. Lo que significa que tiene una infraestructura descentralizada para poder resolver las peticiones de los clientes en su propia región geográfica. Esto lo permite un sistema de sincronismos y cachés que tienen la contrapartida de generar algunos errores de valoración y disponibilidad de reservas.

Este y otros indicadores, aunque son críticos, su accesibilidad está reducida a la consulta a través de un sistema de monitorización como DataDog. __A fin de poder monitorizar de una forma efectiva estas alertas, he ideado un _gadget_ en forma de tarjeta de identificación que, conectado por wifi, traduce en tiempo real las cifras de KPIs a códigos de color y destellos de luz__.

![](https://github.com/McOrts/kpi-booking-gadget/blob/master/images/Gadget_KPI.gif?raw=true)

Este dispositivo representa el _front_ de la aplicación. Tiene un pulsador y tres indicadores luminosos (NeoPixel) que de izquierda a derecha, son para:
- __Error en las confirmaciones de reserva__: 0% verde al 5% rojo.
- __Ventas por el canal B2B2C__: 
    - Se reducen: de blanco a negro
    - Se incrementan: de negro a blanco
- __Alertas planetarias__ indicadas con destellos azules y rojos:
    - Nuevos terremotos en cualquier parte del mundo de cualquier intensidad en la última hora.
    - Post de un nuevo Twitter de Donald Trump (@realDonaldTrump).

Este proyecto representa un ejemplo de cómo la tecnología open-hardware/software puede incorporarse en una gran organización y del bajo coste con el que se puede prototipar hardware.

## Arquitectura
El componente principal que he utilizado en esta arquitectura es un _Broker_ (Mosquitto) de mensajería tipo MQTT. En el _backend_ hay  un orquestador extremadamente sencillo de programar, Node-RED. Como dispositivos periféricos tenemos la tarjeta con los indicadores LED, el teléfono móvil con la pulsera inteligente asociada y un robot modelo OTTO. Todo interconectado con una red Wifi. 

![Arquitectura Gadget KPI](https://github.com/McOrts/kpi-booking-gadget/blob/master/images/Gadget_KPI_arquitectura.png?raw=true)

Las tecnologías y herramientas utilizadas son bien conocidas en el mundo maker para IOT:
* Backend: [Node-RED](https://nodered.org/)
* Broker de colas MQTT: [Eclipse Mosquitto](https://mosquitto.org/)
* Micro-controlador: ESP-8266 formato [WEMOS D1 mini](https://wiki.wemos.cc/products:d1:d1_mini)
* Servidor: [Raspberry Pi Zero Wifi](https://www.raspberrypi.org/products/raspberry-pi-zero/)
* Robot: [Otto](https://www.ottodiy.com/) basado en una versión Wifi no oficial del Arduino Nano.

### Procesos
__La operativa__ de toda la aplicación parte de las consultas que se hacen desde el _backend_ (Node-RED):
1. Se lanza una petición REST la los web-services:
    - DATADOG para los indicadores de negocio. [](https://app.datadoghq.com/api/v1/query?api_key=...)
    - GeoJSON servicio público de alertas globales de terremotos. [](https://earthquake.usgs.gov/earthquakes/feed/v1.0/summary/all_hour.geojson)
    Para componer la petición es necesario utilizar un nodo de programación en Javascript con este código:
```
  var TimeTo = Math.floor((new Date).getTime()/1000);
  var TimeFrom = TimeTo-1800;
  var URLstr1 = "https://app.datadoghq.com/api/v1/query?api_key=";
  var URLstr2 = flow.get('api_key');
  var URLstr3 = "&application_key="
  var URLstr4 = flow.get('application_key');
  var URLstr5 = "&from=";
  var URLstr6 = TimeFrom.toString();
  var URLstr7 = "&to=";
  var URLstr8 = TimeTo.toString();
  var URLstr9 = "&query=sum:evolution.com.evo.servlets.NewShoppingCartConfirmation.processRequest.count{*,env:live,building-block:evolution}.as_count()"
msg.payload = URLstr1.concat(URLstr2,URLstr3,URLstr4,URLstr5,URLstr6,URLstr7,URLstr8,URLstr9);
return [msg];
```
   Así mismo, la respuesta del webservice, aunque se nos entrega parseada por el nodo, tendremos que interpretar el mensaje JSON para lo que tenemos que utilizar otro nódo de función con el código:
```
p=msg.payload;
node.log(typeof P);
var Bookings = 0;
var t = p.series[0].length;
var i;
for (i=0; i<t; i++) {
   Bookings = Bookings + p.series[0].pointlist[i][1];
}
Bookings = Bookings * 2;
msg.payload=Bookings;
msg.topic = "bookings";
return msg;
```
2. Se componen los mensajes para los topics de MQTT que la tarjeta está escuchando. La estructura es así:
```
`-- hbg
    |-- kpi
    |   `-- panic
    |   `-- operation
```
   Por ejemplo, si en Node-RED se detecta a través del _endpoint_ REST del Datadog que las ventas del B2B2C suben. Montará este mensaje MQTT: 
> /hbg/kpi/operation/evolutionUP

   Otros ejemplos de comandos que se pueden generar son: "bookerror21" (2.5% de errores) y "warnings" para un nuevo twitt de Trump.

![Flujo Node-RED del proceso hasta aquí](https://github.com/McOrts/kpi-booking-gadget/blob/master/images/Node-RED_Flow_EVO.PNG?raw=true) 

3. El servidor MQTT (Mosquitto) recibe el mensaje y lo replica a todos los clientes subscritos al topic "/hbg/kpi/operation"
4. El microcontrolador de la tarjeta es uno de esos clientes, por lo que recibe el mensaje e interpreta su _payload_. Por ejemplo "bookerror21" que lo transformará en una instrucción al puerto digital D4 del ESP8266 con un valor RGB (Rojo,verde,azul) de 210,255,0 encendiendo el primer led en un color casi amarillo.

Para el caso del pulsar el _botón "Panic"_ de la tarjeta. Hay que describir un proceso inverso al anterior:
1. El microcontrolador generará este mensaje MQTT:
> /hbg/kpi/panic/on
2. Y en este caso, el _backend_ actúa como un cliente suscrito a este topic. A partir de aquí, con Node-RED podemos hace casi cualquier cosa. En mi caso me envía un mail con un formato que activa las alertas de mi reloj Xiaomi MiBand y activa un icono en el _dashboard_ de la página web servida por Node-RED.

![Flujo Node-RED del proceso de Panic](https://github.com/McOrts/kpi-booking-gadget/blob/master/images/Node-RED_Flow_Panic.PNG?raw=true) 

3. Adicionalmente he añadido la librería del cliente MQTT al programa del robot OTTO de manera que cuando recibe este mensaje ejecuta la rutina del "Cruzaito":

![OTTO](https://github.com/McOrts/kpi-booking-gadget/blob/master/images/OTTO.gif?raw=true)

### El _Dashboard_
Node-RED también tiene una serie de nodos con los que se puede montar una página web responsiva. Es fácil utilizar controles y gráficas, así como cajas de texto, formularios, etc...

![Node-RED Dashboard](https://github.com/McOrts/kpi-booking-gadget/blob/master/images/Gadget_KPI_dashboard.png?raw=true)

El uso es tan simple como dirigir los mensajes a estas 'cajas' y colocarlas en el orden conveniente dentro del _layauut_ del _dashboard_:

![Configuración Node-RED del Dashboard](https://github.com/McOrts/kpi-booking-gadget/blob/master/images/Node-RED_Flow_Dashboard.PNG?raw=true)

## Hardware e instalaciones
### Servidor 
Tanto el _Broker_ MQTT de mensajería (Mosquitto) coo el _backend_ (Node-RED) están instalados en un _One Single Computer_ que es la conocida Raspberry Pi Zero. Se puso a la venta en 2015. Con un coste de cinco dólares. llegándose a regalar en el número #40 de la revista The MagPi. Sus características principales son:
- Procesar ARM 1GHz, single-core CP 
- Sistema operativo: Raspbian es un distribución de Debian
- 512MB de RAM

![Raspberry Pi Zero](https://github.com/McOrts/kpi-booking-gadget/blob/master/images/FreeRPIZero.PNG?raw=true) 

Sin entrar en los detalles de la instalación de este servidor para no hacer demasiado largo este documento y para no repetir lo que se puede encontrar perfectamente explicado, estas son las páginas donde se pueden encontrar las instrucciones adecuadas:
1. [Sistema operativo Raspbian](https://www.raspberrypi.org/documentation/installation/installing-images/README.md)
2. [Node-RED](https://nodered.org/docs/hardware/raspberrypi)
3. [Mosquitto](https://www.instructables.com/id/Installing-MQTT-BrokerMosquitto-on-Raspberry-Pi/)

### La tarjeta
Está construida bajo los principios del _Do It Yourselft_. Con las herramientas básicas para soldar y para trabajar con plásticos y estos materiales. Es posible que cualquier persona pueda construir este _gadget_ por si mismo.

El dispositivo está basado en el microcontrolador SP8266. Cuyo origen está en Shanghai de manos de la empresa Espressif en el 2014.	Pero no nació para usarse como microcontrolador. Si no como controlador Wifi de bajo coste. Este microchip tiene todo el stack TCP/IP que la comunidad Maker adoptó como un dispositivo independiente.

![WEMOS D1 Mini](https://github.com/McOrts/kpi-booking-gadget/blob/master/images/WEMOS-d1-mini-esp8266-board-PINOUT.jpg?raw=true) 

El código fuente en C++ se puede encontrar en [este _sketch_](https://github.com/McOrts/kpi-booking-gadget/tree/master/kpi-booking-gadget-device) de este mismo repositorio. Y este el circuito que puede construirse en muchos formatos:

![BB](https://github.com/McOrts/kpi-booking-gadget/blob/master/images/kpi-booking-gadget-device_bb.png?raw=true) 
![BB](https://github.com/McOrts/kpi-booking-gadget/blob/master/images/Gadget_KPI_rear.png?raw=true) 

## Demo en vivo

  <a href="http://www.youtube.com/watch?feature=player_embedded&v=7N38AHVoB3g
  " target="_blank"><img src="http://img.youtube.com/vi/7N38AHVoB3g/0.jpg"
  alt="Basic video" width="240" height="180" border="10" />
  
## Siguientes pasos
Existe una tecnologia y técnica de fabricación de componentes en superficie llamada SMD (Surface Mount Device) que permite la minituarización de los circuitos impresos (PCB). Y en esta linea estoy trabajando para tener unos dispositivos más robustos que puedan tener un uso diario. 

## Agradecimientos y referencias
- [MQTT.org](http://mqtt.org)
- IBM Developers [MQTT: Enabling the Internet of Things](https://developer.ibm.com/messaging/2013/04/26/mqtt-enabling-internet-things/)
