#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "wi-fi_config.h" // Contains SSID and PASSWORD

WiFiClient client;
String targetIP = "";

void setup() {
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nConnected! IP Address: " + WiFi.localIP().toString());
    
    findOtherDevice();
}

void loop() {
    if (targetIP != "") {
        //Serial.println("test");
        receiveMessage(targetIP, "Open_for_message");


        
    }
    delay(1000);
}


void findOtherDevice() {
    HTTPClient http;
    for (int i = 1; i < 255; i++) {
        String testIP = "192.168.204." + String(i);
        Serial.println("Checking: " + testIP);
        
        if (sendMessage(testIP, "redlight")) {
            Serial.println("Device found at: " + testIP);
            sendMessage(targetIP, "Hello!");
            Serial.println("This is the place.");
            targetIP = testIP;
            break;
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
        //if (response == "100") {
        //  Serial.println("It is a one hundred!");
        //}
    }
    http.end();
    return false;
}

bool receiveMessage(String ip, String message) {
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
        if (response == "100") {
          Serial.println("It is a one hundred!");
        }
        if (response == "150") {
          Serial.println("It is 150!");
        }
    }
    http.end();
    return false;
}


