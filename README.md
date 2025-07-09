# Flood API

To run Swagger UI locally using the Flood API OpenAPI spec, use docker-compose:
```shell
docker-compose up -d --build
```

## Tools
For this solution, I wanted to see how easy it was to create a REST API in C++. Several 
tools exist, but for the purposes of this I chose pistacheio.

> Need to insert other considerations here.

To install Pistache, you can follow this guide, but otherwise it's essentially this:
```shell
git clone https://github.com/pistacheio/pistache.git
cd pistache
meson setup build
meson install -C build
```