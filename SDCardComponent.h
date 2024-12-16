#pragma once
#include "esphome.h"
#include <SPI.h>
#include <SD.h>

class SDCardComponent : public Component {
 public:
  void setup() override {
    // Log the SPI bus initialization
    ESP_LOGI("sdcard", "Setting up SD card on custom SPI bus...");

    // Initialize the SD card with the custom SPI bus and CS pin (GPIO38)
    if (!SD.begin(38)) {  // The CS pin is GPIO38 (or the one defined in your YAML)
      ESP_LOGE("sdcard", "SD card initialization failed!");
    } else {
      ESP_LOGI("sdcard", "SD card initialized successfully!");
    }
  }

  void loop() override {
    // You can implement periodic tasks if needed, e.g., monitoring SD card state
  }

  // Function to read a file from the SD card (in this case, for serving audio files)
  String read_file(const char *path) {
    ESP_LOGI("sdcard", "Reading file: %s", path);
    File file = SD.open(path);

    if (!file) {
      ESP_LOGE("sdcard", "Failed to open file: %s", path);
      return "";
    }

    String file_content;
    while (file.available()) {
      file_content += char(file.read());
    }
    file.close();

    ESP_LOGI("sdcard", "File read successfully.");
    return file_content;
  }

  // Function to serve a file over the web server (for audio playback)
  void serve_file(AsyncWebServerRequest *request, const char *path) {
    ESP_LOGI("sdcard", "Serving file: %s", path);

    if (SD.exists(path)) {
      File file = SD.open(path);
      request->send(file, path, "audio/mpeg");  // Change mime type based on the file type (e.g., audio/mpeg for MP3)
      file.close();
    } else {
      request->send(404, "text/plain", "File not found");
      ESP_LOGE("sdcard", "File not found: %s", path);
    }
  }
};
