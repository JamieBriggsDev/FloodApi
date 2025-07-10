# Flood API

To run Swagger UI locally using the Flood API OpenAPI spec, use docker-compose:
```shell
docker-compose up -d --build
```

## Tools
For this solution, I wanted to see how easy it was to create a REST API in C++. Several 
tools exist, but for the purposes of this I chose pistacheio.

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