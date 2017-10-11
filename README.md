# README #

Простая плата на базе микросхемы ESP8266 (Wemos D1 mini). 

Отображение в web интерфейсе и отправка на сервер умного дома по протоколу MQTT


FileSystem Install
[https://github.com/esp8266/arduino-esp8266fs-plugin]

EasyEDA Project
[https://easyeda.com/AKEB/New_Project-71627d214a5542078ba6a168b7f5fc28]


# Распиновка #


## Ссылки ##
* [esptool-gui](https://github.com/Rodmg/esptool-gui)
* [esptool](https://github.com/espressif/esptool)


## Код для заливки прошивки ##
```
#!bash

./esptool.py --port /dev/cu.wchusbserial1420 write_flash -fm=dio -fs=4MB 0x00000 ~/Downloads/0x00000_ESP8266_201705062105.bin

```