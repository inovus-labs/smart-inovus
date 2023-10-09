#include <WiFiClientSecure.h>
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>  // Include the ArduinoJSON library

using namespace websockets;

WebsocketsClient client;
WiFiClient espClient;

const char* ssid = "master";
const char* password = "123456789";

const char ssl_cert[] =  "-----BEGIN CERTIFICATE-----\n" \
  "MIIFYDCCBEigAwIBAgIQQAF3ITfU6UK47naqPGQKtzANBgkqhkiG9w0BAQsFADA/\n" \
  "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
  "DkRTVCBSb290IENBIFgzMB4XDTIxMDEyMDE5MTQwM1oXDTI0MDkzMDE4MTQwM1ow\n" \
  "TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n" \
  "cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwggIiMA0GCSqGSIb3DQEB\n" \
  "AQUAA4ICDwAwggIKAoICAQCt6CRz9BQ385ueK1coHIe+3LffOJCMbjzmV6B493XC\n" \
  "ov71am72AE8o295ohmxEk7axY/0UEmu/H9LqMZshftEzPLpI9d1537O4/xLxIZpL\n" \
  "wYqGcWlKZmZsj348cL+tKSIG8+TA5oCu4kuPt5l+lAOf00eXfJlII1PoOK5PCm+D\n" \
  "LtFJV4yAdLbaL9A4jXsDcCEbdfIwPPqPrt3aY6vrFk/CjhFLfs8L6P+1dy70sntK\n" \
  "4EwSJQxwjQMpoOFTJOwT2e4ZvxCzSow/iaNhUd6shweU9GNx7C7ib1uYgeGJXDR5\n" \
  "bHbvO5BieebbpJovJsXQEOEO3tkQjhb7t/eo98flAgeYjzYIlefiN5YNNnWe+w5y\n" \
  "sR2bvAP5SQXYgd0FtCrWQemsAXaVCg/Y39W9Eh81LygXbNKYwagJZHduRze6zqxZ\n" \
  "Xmidf3LWicUGQSk+WT7dJvUkyRGnWqNMQB9GoZm1pzpRboY7nn1ypxIFeFntPlF4\n" \
  "FQsDj43QLwWyPntKHEtzBRL8xurgUBN8Q5N0s8p0544fAQjQMNRbcTa0B7rBMDBc\n" \
  "SLeCO5imfWCKoqMpgsy6vYMEG6KDA0Gh1gXxG8K28Kh8hjtGqEgqiNx2mna/H2ql\n" \
  "PRmP6zjzZN7IKw0KKP/32+IVQtQi0Cdd4Xn+GOdwiK1O5tmLOsbdJ1Fu/7xk9TND\n" \
  "TwIDAQABo4IBRjCCAUIwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYw\n" \
  "SwYIKwYBBQUHAQEEPzA9MDsGCCsGAQUFBzAChi9odHRwOi8vYXBwcy5pZGVudHJ1\n" \
  "c3QuY29tL3Jvb3RzL2RzdHJvb3RjYXgzLnA3YzAfBgNVHSMEGDAWgBTEp7Gkeyxx\n" \
  "+tvhS5B1/8QVYIWJEDBUBgNVHSAETTBLMAgGBmeBDAECATA/BgsrBgEEAYLfEwEB\n" \
  "ATAwMC4GCCsGAQUFBwIBFiJodHRwOi8vY3BzLnJvb3QteDEubGV0c2VuY3J5cHQu\n" \
  "b3JnMDwGA1UdHwQ1MDMwMaAvoC2GK2h0dHA6Ly9jcmwuaWRlbnRydXN0LmNvbS9E\n" \
  "U1RST09UQ0FYM0NSTC5jcmwwHQYDVR0OBBYEFHm0WeZ7tuXkAXOACIjIGlj26Ztu\n" \
  "MA0GCSqGSIb3DQEBCwUAA4IBAQAKcwBslm7/DlLQrt2M51oGrS+o44+/yQoDFVDC\n" \
  "5WxCu2+b9LRPwkSICHXM6webFGJueN7sJ7o5XPWioW5WlHAQU7G75K/QosMrAdSW\n" \
  "9MUgNTP52GE24HGNtLi1qoJFlcDyqSMo59ahy2cI2qBDLKobkx/J3vWraV0T9VuG\n" \
  "WCLKTVXkcGdtwlfFRjlBz4pYg1htmf5X6DYO8A4jqv2Il9DjXA6USbW1FzXSLr9O\n" \
  "he8Y4IWS6wY7bCkjCWDcRQJMEhg76fsO3txE+FiYruq9RUWhiF1myv4Q6W+CyBFC\n" \
  "Dfvp7OOGAN6dEOM4+qR9sdjoSYKEBpsr6GtPAQw4dy753ec5\n" \
  "-----END CERTIFICATE-----\n";;  

// Define pin numbers for your devices
const int ceiling_light_1_pin = 2;
const int ceiling_light_2_pin = 3;
const int strip_light_1_pin = 4;
const int strip_light_2_pin = 5;
const int fan_pins[] = {6, 7, 8, 9, 10, 11};

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setCACert(ssl_cert);
  bool connected = client.connect("ws://smart-inovus-server.glitch.me/");

  if (connected) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
  }

  client.onMessage([&](WebsocketsMessage message) {
    Serial.print("Got Message: ");
    Serial.println(message.data());

    // Parse the received JSON message
    StaticJsonDocument<200> jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, message.data());

    if (!error) {
      // Check and control devices based on JSON data
      if (jsonDoc["ceiling_light_1"]) {
        digitalWrite(ceiling_light_1_pin, HIGH);
      } else {
        digitalWrite(ceiling_light_1_pin, LOW);
      }

      if (jsonDoc["ceiling_light_2"]) {
        digitalWrite(ceiling_light_2_pin, HIGH);
      } else {
        digitalWrite(ceiling_light_2_pin, LOW);
      }

      if (jsonDoc["strip_light_1"]) {
        digitalWrite(strip_light_1_pin, HIGH);
      } else {
        digitalWrite(strip_light_1_pin, LOW);
      }

      if (jsonDoc["strip_light_2"]) {
        digitalWrite(strip_light_2_pin, HIGH);
      } else {
        digitalWrite(strip_light_2_pin, LOW);
      }

      for (int i = 0; i < sizeof(fan_pins) / sizeof(fan_pins[0]); i++) {
        if (jsonDoc["fan_" + String(i + 1)]) {
          digitalWrite(fan_pins[i], HIGH);
        } else {
          digitalWrite(fan_pins[i], LOW);
        }
      }
    } else {
      Serial.print("JSON parsing failed: ");
      Serial.println(error.c_str());
    }
  });
}

void loop() {
  client.poll();

  if (WiFi.status() != WL_CONNECTED) {
    bool webSocketConnection = false;

    Serial.print("Reconnecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("");

    while (!webSocketConnection) {
      bool connected = client.connect("wss://smart-inovus-server.glitch.me/");
      if (connected) {
        Serial.println("Connected");
        webSocketConnection = true;
      } else {
        Serial.println("Connection failed.");
      }
      delay(2000);
    }
  }
}
