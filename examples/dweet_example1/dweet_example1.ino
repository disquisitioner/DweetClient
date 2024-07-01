#include <SensorHelper.h>

#include <WiFi.h>
#include <ArduinoJson.h>
#include "DweetClient.h"

// Need to connect to WiFi to demonstrate DweetClient. Substitute
// your local wifi credentials here
#define WIFI_SSID "WIFI-SSID"
#define WIFI_PASSWORD "WIFI-PASSWORD"

// Modify to suit your tastes (though DWEET_HOST should be "dweet.io")
#define DWEET_HOST "dweet.io"
#define DWEET_DEVICE "dweetclient-example"

// Create DweetClient object in verbose mode
DweetClient dweeter(DWEET_HOST,DWEET_DEVICE,true);

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to network via WiFi
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("WiFi RSSI: ");
  Serial.println(WiFi.RSSI());
}

uint16_t counter = 1;
void loop() {
  JsonDocument jsondata;

  // Simple loop that Dweets periodically
  jsondata["network"] = String(WIFI_SSID);
  jsondata["wifi_rssi"] = String(WiFi.RSSI());
  jsondata["device"] = String(DWEET_DEVICE);
  jsondata["counter"] = counter;

  // Post our dweet!
  dweeter.postPayloadJSON(jsondata);

  counter++;
  delay(30000);  // Delay 30 seconds
}
