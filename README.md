# Flood API

To get started, clone the repository.

```shell
git clone https://github.com/JamieBriggsDev/FloodApi.git
git submodule init
git submodule update
```

To run Swagger UI locally using the Flood API OpenAPI spec, use docker-compose:

```shell
docker-compose up -d --build
```

## API Setup

Before building the package, a file needs creating for the WIFI settings. Copy the existing example header file:

```shell
cp api/include/wifi_settings.h.example api/include/wifi_settings.h
```

This file then needs populating with your WiFi SSID and password. Refrain from pushing this file up.

## Tools (WIP)

For this solution, I wanted to see how easy it was to create a REST API in C++. Several
tools exist, but for the purposes of this I chose aWot.

> Need to insert other considerations here.

To install Pistache, you can follow this guide, but otherwise you can clone the repo and use `meson` to build and install it. If you do not have meson, you can install via brew.

```shell
git clone https://github.com/pistacheio/pistache.git
cd pistache
meson setup build
meson install -C build
```

To install PlatformIO, which is used to flash an ESP32 microcontroller, use homebrew:

```shell
brew install paltformio
```

There is also the PlatformIO plugin for CLion. Guidance can be found [here](https://www.jetbrains.com/help/clion/platformio.html).

For ESP32 related tools, install esp32:

```shell
brew install espressif
```

Drivers for ESP32 and serial found [here](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/linux-macos-setup.html).

Need to modify `MySQL_Packet.h` to work with WiFi found [here](https://github.com/ChuckBell/MySQL_Connector_Arduino/wiki/Hardware-Guide#modifying-the-connector):
```c++
#ifdef ARDUINO_ARCH_ESP32 
    #include <Arduino.h> 
#elif ARDUINO_ARCH_ESP8266
    #include <ESP8266WiFi.h>
#else
    #include <WiFi.h>
//    #include <Ethernet.h>
#endif
```


May also need this:
```sql
-- First ensure the user exists
CREATE USER IF NOT EXISTS 'floodUser'@'%';
ALTER USER 'floodUser'@'%' IDENTIFIED WITH mysql_native_password BY 'floodPassword';
FLUSH PRIVILEGES;
```