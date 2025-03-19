#include <ESP8266WiFi.h>
// - - - - - - - - - - - - - - - - (Begin)
// LED Control Code
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// - - - - - - - - - - - - - - - - (End)
#include <ESP8266HTTPClient.h>
#include "wi-fi_config.h" // Contains WIFI_SSID and WIFI_PASS

WiFiClient client;
String targetIP = "";
// - - - - - - - - - - - - - - - - (Begin)
// LED Control Code
// Define pins for LEDs
    const int redLED = 16;
    const int yellowLED = 14;
    const int greenLED = 12;
// - - - - - - - - - - - - - - - - (End)

void setup() {
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    // - - - - - - - - - - - - - - - - (Begin)
    // LED Control Code
          // Set each LED pin as an OUTPUT
          pinMode(redLED, OUTPUT);
          pinMode(yellowLED, OUTPUT);
          pinMode(greenLED, OUTPUT);
        //}
    // - - - - - - - - - - - - - - - - (End)
    Serial.println("\nConnected! IP Address: " + WiFi.localIP().toString());
    findOtherDevice();
}

void loop() {
    if (targetIP != "") {
        String message = "test";  
        HTTPClient http;
        String url = "http://" + targetIP + "/message?data=" + message;  
        http.begin(client, url);  
        int httpCode = http.GET();
        if (httpCode > 0) {
            String response = http.getString();
            Serial.println("Response from " + targetIP + ": " + response);
            if (response == "green") {
                digitalWrite(redLED, LOW);
                digitalWrite(yellowLED, LOW);
                digitalWrite(greenLED, HIGH);
            }
            if (response == "yellow") {
                digitalWrite(redLED, LOW);
                digitalWrite(yellowLED, HIGH);
                digitalWrite(greenLED, LOW);
            }
            if (response == "red") {
                digitalWrite(redLED, HIGH);
                digitalWrite(yellowLED, LOW);
               digitalWrite(greenLED, LOW);
            }
            if (response == "") {  // (No response from the "distance sensor" Arduino.)
              digitalWrite(redLED, HIGH);    // Blinks the red LED on and then turns it off  
              digitalWrite(yellowLED, LOW);  // to show that the "stoplight" Arduino didn't 
              digitalWrite(greenLED, LOW);   // receive a response from the "distance sensor"
              delay(1000);                   // one.
              digitalWrite(redLED, LOW);   
            }
        }
        http.end();
    }
    delay(1);
}

void findOtherDevice() {
    HTTPClient http;
    for (int i = 1; i < 255; i++) {    // Middle value should be "255".
        String testIP = "192.168.204." + String(i);
        Serial.println("Checking: " + testIP);
        digitalWrite(redLED, HIGH);  // Blinks the red LED on and off to indicate
        delay(500);                  // that the "stoplight" Arduino is looking
        digitalWrite(redLED, LOW);   // for the "distance sensor" one.
        if (sendMessage(testIP, "redlight")) {
            Serial.println("Device found at: " + testIP);
            sendMessage(targetIP, "Hello!");
            targetIP = testIP;
            break;
        }
        if (i == 254) {                     // The solid yellow LED indicates that the "stoplight" 
          digitalWrite(yellowLED, HIGH);  // Arduino was unable to find the "distance sensor" one.
        }
    }
}

bool sendMessage(String ip, String message) {
    HTTPClient http;
    String url = "http://" + ip + "/message?data=" + message;
    http.begin(client, url);
    int httpCode = http.GET();
    if (httpCode > 0) {
        String response = http.getString();
        Serial.println("Response from " + ip + ": " + response);
        if (response == "greenlight") {
            return true;
        }
    }
    http.end();
    return false;
}

