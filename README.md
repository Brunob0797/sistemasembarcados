# Smart Green Tech Irrigation

Este projeto busca utilizar um algor�tmo KNN para verificar a necessidade de irrigar uma planta��o.

### Requisitos

Este software foi desenvolvido para o funcionamento em uma BeagleBone Black, todas as bibliotecas necess�rias est�o incluidas no mesmo, al�m disso, s�o necess�rios os seguintes sensores e componentes:

* 1 BeagleBone Black
* 1 Sensor de temperatura e �midade DHT22
* 1 Sensor de �midade do solo higr�metro
* 1 Display OLED 0.96" I2C
* 1 M�dulo rel�
* 1 V�lvula Solen�ide de Entrada de �gua
* 1 resistor de 4700 ohms

### Instala��o

Aqui temos um passo a passo de como compilar a aplica��o para a sua BeagleBone Black:

- Inicialmente � necess�rio entrar na pasta raiz da aplica��o compilar a biblioteca BlackLib, para isso basta rodar o seguinte comando no terminal:

```
g++ -c v2_0/BlackCore.cpp v2_0/BlackGPIO.cpp
```

Depois disso � necess�rio compilar o restanto de c�digo:

```
g++ bbb_dht_read.c bbb_mmio.c common_dht_read.c I2C.c SSD1306_OLED.c example_app.c v2_0/BlackGPIO.o v2_0/BlackCore.o codigo.cpp -o app
```

## Rodando a aplica��o

Para iniciar a aplica��o basta executar o app, para isso utilize o comando:

```
./app


### Instala��o

Para o funcionamento dos sensores, seguindo a aplica��o, eles devem ser conectados nas seguintes portas:

* Sensor DHT22 - Porta P8_11
* Sensor de �midade do solo higr�metro - Porta P9_39
* Display OLED - SCL - Porta P9_19
* Display OLED - SDA - Porta P9_20
* M�dulo rel� - Porta GPIO_66

## V�deo explicativo

Para entender o funcionamento do projeto foi feito um v�deo explicativo:

* [Youtube](https://youtu.be/NgO0qdY4kBM)


## Sistema Operacional

Na BeagleBone Black foi utilizado o sistema operacional Debian 9.5 2018-10-07, dispon�vel no site do fabricante

## Autores

* **Bruno Barcarolo** - [Git](https://github.com/Brunob0797)
* **Carlos Eduardo** - [Git](https://github.com/Ziiroo)
* **Ighor Lameira** - [Git](https://github.com/ighorgl)
* **Jo�o Vitor Zago** - [Git](https://github.com/jvzago)

