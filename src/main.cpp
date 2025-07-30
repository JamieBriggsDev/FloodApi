//
// Created by Jamie Briggs on 10/07/2025.
//

#include <Arduino.h>
#include <sstream>

#include "../src/display/LCDDisplay.h"
#include "../src/display/LiquidCrystalAdapter.h"
#include "FloodRoutes.h"
#include "config/def_pin_outs.h"
#include "config/def_wifi_settings.h"
#include "db/FloodRepository.h"
#include "logger/def_logger_factory.h"
#include "mapper/FloodMapper.h"


routes::FloodRoutes* flood_routes;
common::display::LiquidCrystalAdapter* lcd;
common::display::LCDDisplay* display;
db::IFloodRepository* flood_repository;
mapper::IFloodMapper* flood_mapper;

void setup()
{
  Serial.begin(115200);
  LOG.info("Setting up Flood API...");

  LOG.info("Checking defragmentation...");
  // In your setup(), before initializing your components
  if (heap_caps_check_integrity_all(true))
  {
    LOG.debug("Heap integrity check passed");
  }

  LOG.debug("Initializing LCD...");
  lcd = new common::display::LiquidCrystalAdapter(jbriggs::flood::config::ESP_RS_PIN, jbriggs::flood::config::ESP_ENABLE_PIN, jbriggs::flood::config::ESP_D0_PIN,
                                                  jbriggs::flood::config::ESP_D1_PIN, jbriggs::flood::config::ESP_D2_PIN, jbriggs::flood::config::ESP_D3_PIN);
  LOG.debug("Initializing Printer Service...");
  display = new common::display::LCDDisplay(*lcd);
  display->displayText("Starting", "Flood App!", common::display::STICKY);
  LOG.debug_f("Initial Free Heap: %d bytes", ESP.getFreeHeap());


  LOG.debug("Creating Flood repository...");
  flood_repository = new db::FloodRepository("/flood_downgraded.db");


  LOG.debug("Initializing Flood mapper...");
  flood_mapper = new mapper::FloodMapper();


  LOG.debug_f("Connecting to WiFi: %s", flood::config::WIFI_SSID);
  WiFiClass::mode(WIFI_STA);
  WiFi.begin(flood::config::WIFI_SSID, flood::config::WIFI_PASSWORD);
  while (WiFiClass::status() != WL_CONNECTED)
  {
    delay(500);
    display->displayText("Connecting..", common::display::FLASH);
  }

  // Display IP and PORT number
  std::ostringstream portMessage;
  portMessage << "Port: " << std::to_string(flood::config::PORT);
  display->displayText(WiFi.localIP().toString().c_str(), portMessage.str(), common::display::STICKY);

  LOG.debug("Initializing Flood routes...");
  flood_routes = new routes::FloodRoutes(display, flood_repository, flood_mapper);


  LOG.debug("Initializing Flood repository...");
  // This is initialized after FloodRoutes has initialized
  flood_repository->init();

  LOG.info("Completed setup!");
}

void loop()
{
  // Monitor heap memory usage
  LOG.debug_f("Free Heap: %d KB, Out of: %d KB", ESP.getFreeHeap() / 1024, ESP.getHeapSize() / 1024);

  flood_routes->loop();

  delay(1000); // Wait 5 seconds
}

void cleanup()
{
  delete lcd;
  delete display;
  delete flood_repository;
  delete flood_mapper;
  delete flood_routes;
}
