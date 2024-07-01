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

#include "Arduino.h"
#include <HTTPClient.h>
#include "DweetClient.h"

DweetClient::DweetClient(String server, String device, bool verbose = true) {
  _server = server;
  _device = device;
  _verboseMode = verbose;
}

void DweetClient::setVerbose(bool verbose) {
  _verboseMode = verbose;
}

// One way to provide the data to be dweeted is to pack it in a valid JSON document
void DweetClient::postPayloadJSON(JsonDocument payloadJSON) {
  int httpCode;
  String payloadString;

  // Serialize the JSON payload into a String and pass it to the postPayloadString() function
  serializeJson(payloadJSON,payloadString);
  postPayloadString(payloadString);
}

// Another way to provide the data to be dweeted is to construct a string that is valid JSON,
// Which is easy enough if there aren't many values to be dweeted, but for more data rich
// use is less convenient than passing a JSON document payload (see above)
void DweetClient::postPayloadString(String payloadString) {
  int httpCode;
  String dweeturl;

  // Construct the DWEET URL to be used for this device
  // Posting to "/dweet/for/{thing}" yields a verbose response confirming success and
  // echoes the dweet payload received. Posting to "dweet/quietly/for/{thing}" results
  // in just an HTTP 204 return code if successful rather than the typical verbose response.
  if(_verboseMode) {
    dweeturl = "http://" + _server + "/dweet/for/" + _device;
  } else {
    dweeturl = "http://" + _server + "/dweet/quietly/for/" + _device;
  }
  // Post the resquest to the target server and pass the JSON payload in String form
  httpCode = _httpPOSTRequest(dweeturl,"application/json",payloadString);

  // httpCode will be negative on error, but HTTP status might indicate failure
  if (httpCode > 0) {
    // HTTP POST complete, print result code
    Serial.println("HTTP POST [" + dweeturl + "], result code: " + String(httpCode));
  } else {
    Serial.println("HTTP POST [" + dweeturl + "] failed, result code: " + String(httpCode));
  }
}

// Local convenience function for handling the HTTP POST to Dweet.  Might be useful generally
// but for now is private to the DweetClient class
int DweetClient::_httpPOSTRequest(String serverurl, String contenttype, String payload)
{
  int httpCode = -1;
  HTTPClient http;

  // There are a variety of ways to initiate an HTTP operation, the easiest of which
  // is just to specify the target server URL.
  http.begin(serverurl);
  http.addHeader("Content-Type", contenttype);  // Add our supplied content type

  // Now POST the supplied payload
  httpCode = http.POST(payload);
  
  // httpCode will be negative on error, but HTTP status might indicate failure
  if (httpCode > 0) {
    // If POST succeeded (status 200), output any response as it might be useful to
    // the calling application and need to do so before closing server connection
    if (httpCode == HTTP_CODE_OK) {
      const String& response = http.getString();
      Serial.println("OK, received response:\n<<");
      Serial.println(response);
      Serial.println(">>");
    }
    // For other HTTP status codes we'll just return httpCode
  } 
  // If HTTP error provide reason associated with HTTP status returned 
  else {
    Serial.println("HTTP POST [" + serverurl + "] failed, error: " + http.errorToString(httpCode).c_str());
  }

  http.end();  // Closes connection to host begun above
  // Serial.println("closing connection to host"); 
  return(httpCode);
}