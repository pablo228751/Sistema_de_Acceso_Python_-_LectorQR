<h1 align="center">Sistema de Acceso con Servidor en Python + Lector QR</h1>

<p align="center">
  <img src="imagen1.jpg" alt="Placas en la torre de la Barrera">
  <img src="imagen2.jpg" alt="Lector QR">
</p>

<p align="center">
  Las placas Arduino utilizan varias bibliotecas para realizar diferentes tareas. El objetivo de este programa es controlar una barrera que se abre y se cierra en función de los datos recibidos a través de una conexión TCP/IP (servidor python) y una comunicación serie.

Las bibliotecas utilizadas son las siguientes:

* avr/wdt.h: Biblioteca para manejar el temporizador de vigilancia (Watchdog Timer), que es un dispositivo de hardware que puede reiniciar automáticamente el microcontrolador si se produce algún error en el programa.
* SoftwareSerial.h: Biblioteca para comunicación serie de software.
* Ethernet.h: Biblioteca para usar el módulo Ethernet de Arduino.
* SPI.h: Biblioteca que proporciona una interfaz para comunicarse con dispositivos a través del protocolo SPI.
* SD.h: Biblioteca para usar una tarjeta SD con Arduino.
El programa comienza con la definición de una dirección MAC, una dirección IP, una dirección de DNS, una dirección de gateway y una máscara de subred. A continuación, se crea un servidor Ethernet en el puerto 999.

En el método setup(), se deshabilita el temporizador de vigilancia, se inician las comunicaciones serie y Ethernet, se configura el pin de salida para la barrera y se comprueba si la tarjeta SD está funcionando correctamente.

En el método loop(), se llama a la función serialEvento2() para recibir datos de la comunicación serie. Si se recibe un mensaje completo, se llama a la función analizarDatos() para procesar los datos y abrir la barrera si es necesario. A continuación, se llama a la función comunicacionTCP() para recibir datos a través de la conexión TCP/IP y se restablece el temporizador de vigilancia.

También hay funciones para abrir la barrera y para leer y escribir en la tarjeta SD.
</p>


## Videos

[![Ciclo2 del programa](https://img.youtube.com/vi/shorts/i5eDsn3USb0?feature=share)](https://youtube.com/shorts/i5eDsn3USb0?feature=share)

[![Ciclo del programa](https://img.youtube.com/vi/y_g7Ybj4uo8/0.jpg)](https://www.youtube.com/watch?v=y_g7Ybj4uo8)








## Licencia

Mi proyecto genial está bajo la licencia MIT. Ver `LICENSE` para más información.
