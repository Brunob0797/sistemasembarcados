# Smart Green Tech Irrigation

Este projeto busca utilizar um algorítmo KNN para verificar a necessidade de irrigar uma plantação.

### Requisitos

Este software foi desenvolvido para o funcionamento em uma BeagleBone Black, todas as bibliotecas necessárias estão incluidas no mesmo, além disso, são necessários os seguintes sensores e componentes:

* 1 BeagleBone Black
* 1 Sensor de temperatura e umidade DHT22
* 1 Sensor de umidade do solo higrômetro
* 1 Display OLED 0.96" I2C
* 1 Módulo relé
* 1 Válvula Solenóide de Entrada de Água
* 1 resistor de 4700 ohms

### Arquitetura do Projeto

![Arquitetura](Brunob0797.github.com/sistemasembarcados/Arquitetura%20do%20Projeto.jpeg)

### Instalação

Aqui temos um passo a passo de como compilar a aplicação para a sua BeagleBone Black:

Inicialmente é necessário entrar na pasta raiz da aplicação compilar a biblioteca BlackLib, para isso basta rodar o seguinte comando no terminal:

```
g++ -c v2_0/BlackCore.cpp v2_0/BlackGPIO.cpp
```

Depois disso é necessário compilar o restanto de código:

```
g++ bbb_dht_read.c bbb_mmio.c common_dht_read.c I2C.c SSD1306_OLED.c example_app.c v2_0/BlackGPIO.o v2_0/BlackCore.o codigo.cpp -o app
```
### Bibliotecas 3rd party

Algumas bibliotecas externas foram modificadas para o funcionamento do projeto, porém os repositórios originais estão contidos nos seguintes git's:

- [BlackLib](https://github.com/yigityuce/BlackLib)
- [Sensor DHT22](https://github.com/adafruit/Adafruit_Python_DHT)
- [Display OLED](https://github.com/deeplyembeddedWP/SSD1306-OLED-display-driver-for-BeagleBone)


## Rodando a aplicação

Para iniciar a aplicação basta executar o app, para isso utilize o comando:

```
./app
```


### Instalação

Para o funcionamento dos sensores, seguindo a aplicação, eles devem ser conectados nas seguintes portas:

* Sensor DHT22 - Porta P8_11
* Sensor de umidade do solo higrômetro - Porta P9_39
* Display OLED - SCL - Porta P9_19
* Display OLED - SDA - Porta P9_20
* Módulo relé - Porta GPIO_66

## Vídeo explicativo

Para entender o funcionamento do projeto foi feito um vídeo explicativo:

* [Youtube](https://youtu.be/NgO0qdY4kBM)


## Sistema Operacional

Na BeagleBone Black foi utilizado o sistema operacional Debian 9.5 2018-10-07, disponível no site do fabricante

## Autores

* **Bruno Barcarolo** - [Git](https://github.com/Brunob0797)
* **Carlos Eduardo** - [Git](https://github.com/Ziiroo)
* **Ighor Lameira** - [Git](https://github.com/ighorgl)
* **João Vitor Zago** - [Git](https://github.com/jvzago)

