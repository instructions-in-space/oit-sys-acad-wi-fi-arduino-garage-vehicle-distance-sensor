#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "wi-fi_config.h"

ESP8266WebServer server(80);

void setup() {
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnected! IP Address: " + WiFi.localIP().toString());
    
    server.on("/message", handleMessage);
    server.begin();
}

void loop() {
    server.handleClient();
}

void handleMessage() {
    if (server.hasArg("data")) {
        String data = server.arg("data");
        Serial.println("Received: " + data);
        
        if (data == "redlight") {
            server.send(200, "text/plain", "greenlight");
        //} else {
            //server.send(200, "text/plain", "Message received!");
        }
        //if (data == "test") {
          char message_to_send[] = "23";  //was "150"
          server.send(200, "text/plain", message_to_send);
          //server.send(200, "text/plain", "150");
          Serial.println("Sent a one-fifty.");
        //}
    } else {
        server.send(400, "text/plain", "No message received");
    }
}

// Note:  what messages does the "stoplight" Arduino need to send to the "distance-sensor"
// one?  The only thing I can think of would be verifying the connection, and that's 
// important for safety reasons (so I'll need to build that in as well.)



