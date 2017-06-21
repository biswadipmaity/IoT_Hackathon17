#include "Arduino.h"
#include <ESP8266wifi.h>
#include <ESP8266HTTPClient.h>

// WiFi settings
const char *ssid = "Lumos";
const char *password = "password";
char MAC_char[18];
String requestUrl = "http://debugrobotics.azurewebsites.net/Home/SetIP?clientName=iotdemo&clientIp=";

void fillMacAddress()
{
    byte mac[6];
    WiFi.macAddress(mac);

    for (int i = 0; i < 6; ++i) {
        requestUrl += String(mac[i], 16);
        if (i < 5)
            requestUrl += "%3A";
    }
}

void upload_Mac()
{
    Serial.print("Request : ");
    Serial.println(requestUrl);

    HTTPClient http;
    http.begin(requestUrl);

    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0)
    {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString();
            Serial.println(payload);
        }
    }
    else
    {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
}
void setup()
{
    Serial.begin(9600);
    Serial.println("ESP8266 starting in normal mode");
    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    // Print the IP address
    Serial.println(WiFi.localIP());

    fillMacAddress();
    upload_Mac();

    pinMode(BUILTIN_LED, OUTPUT);
}

void loop()
{
    digitalWrite(BUILTIN_LED, HIGH);
    delay(2000);
    digitalWrite(BUILTIN_LED, LOW);
    delay(2000);
}