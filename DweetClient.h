/*
 * A simple messaging client for sharing device information via Dweet.io
 * Author: David Bryant (david@disquiry.com)
 * Date: June 29, 2024
 *
 * This is an Arduino library that provides basic tools for an IoT device
 * to use in publishing messages to Dweet.io. 
 *
 * It depends upon and follows the basic form of the Arduino HTTP Client
 * library, using HTTP POST to send messages to Dweet.io. It also uses
 * The ArduinoJson library for formatting data to be sent in each dweet.
 *
 * The current form generates some output via Serial(), which may contain
 * useful information related to the result of the HTTP POST operation.
 *
 */

#ifndef DWEETCLIENT_H
#define DWEETCLIENT_H

#include "Arduino.h"
#include <ArduinoJson.h>

class DweetClient {
  public:
    DweetClient(String server, String device, bool verbose);
    void setVerbose(bool verbose);
    void postPayloadJSON(JsonDocument payload);
    void postPayloadString(String payloadString);
  private:
    int _httpPOSTRequest(String serverurl, String contenttype, String payload);
    String _server;
    String _device;
    bool _verboseMode;
};

#endif // DWEETCLIENT_H