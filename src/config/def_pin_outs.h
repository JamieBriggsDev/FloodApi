//
// Created by Jamie Briggs on 15/07/2025.
//

#ifndef PINOUTS_H
#define PINOUTS_H

namespace jbriggs::flood::config
{
  constexpr int ESP_RS_PIN = 17;
  constexpr int ESP_ENABLE_PIN = 16;
  constexpr int ESP_D0_PIN = 25;
  constexpr int ESP_D1_PIN = 26;
  constexpr int ESP_D2_PIN = 13;
  constexpr int ESP_D3_PIN = 14;
  // MicroSD Module points
  constexpr int MICRO_SD_CS_PIN = 4;
  // These three pins are standard on ESP32-E and should not be modified
  constexpr int MICRO_SD_MOSI_PIN = 23;
  constexpr int MICRO_SD_MISO_PIN = 19;
  constexpr int MICRO_SD_SCK_PIN = 18;
} // namespace jbriggs::config


#endif // PINOUTS_H
