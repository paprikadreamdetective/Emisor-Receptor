# Lab7: transmisor/receptor IR
# Descripción del proyecto

El funcionamiento de este proyecto se basa en grabar un programa en la placa de desarrollo Blue Pill de cada uno de los integrantes. El primer programa tiene como objetivo enviar una señal a través de un sensor emisor. Esta señal contiene el voltaje establecido mediante un potenciómetro. En el segundo programa, se espera recibir una señal a través de un sensor receptor, y en respuesta, se encenderá un LED con el voltaje recibido. En ambos programas, se debe imprimir en una pantalla el valor del voltaje establecido por el potenciómetro asi como también debera contar con un botón para el encendido y el apagado del LED.

La biblioteca que se suo para este proyecto fue IRMP, la cual provee metodos para poder decodificar y recibir señales infrarrojas utilizando diferentes protocolos, en este proyecto usamos un protocolo de comunicacion NEC, el cual es uno de los tantos que provee esta libreria.
# Compilar el software

Es necesario verificar el funcionamiento de la placa Blue Pill y del ST-Link V2. También utilizaremos una extensión de Visual Studio Code llamada "PlatformIO", la cual nos ayudará a compilar y cargar el programa.

En algunos casos, puede ser necesario mover el jumper para poder cargar el programa, pero al parecer, en Windows esto no es necesario.


#  Diagramas

### Diagrama emisor

![Imagen](https://i.imgur.com/EcnRIDD.png)


### Diagrama receptor

![Imagen](https://i.imgur.com/bxtbZ3O.png)

