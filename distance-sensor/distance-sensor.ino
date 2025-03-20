#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "wi-fi_config.h"

// - - - - - - - - - - - - - - - - (Begin)
// Distance Sensor Code
#define ECHO D2                 // attach pin D2 Arduino to pin Echo of HC-SR04
#define TRIG D3                 // attach pin D3 Arduino to pin Trig of HC-SR04

long duration;                  // variable for the duration of sound wave travel
// - - - - - - - - - - - - - - - - (End)

ESP8266WebServer server(80);

void setup() {
    // - - - - - - - - - - - - - - - - (Begin)
    // Distance Sensor Code
    pinMode(TRIG, OUTPUT);        // Sets the TRIG as an OUTPUT
    pinMode(ECHO, INPUT);         // Sets the ECHO as an INPUT
    //Serial.begin(9600);           // Serial Communication is starting with 9600 of baudrate speed
    Serial.println("HC-SR04 Distance Sensor"); // print some text in Serial Monitor
    // - - - - - - - - - - - - - - - - (End)
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

float inches;

void loop() {
    server.handleClient();
    // - - - - - - - - - - - - - - - - (Begin)
    // Distance Sensor Code
    digitalWrite(TRIG, LOW);    // Turn off the trigger and let things quiet down
    delay(20);                  // Let it sit for 20 milliseconds
    digitalWrite(TRIG, HIGH);   // Turn on the trigger to start measurement
    delayMicroseconds(15);      // Send a very short pulse (15us)
    digitalWrite(TRIG, LOW);    // Turn off TRIG pin - this will start the ECHO pin high
    duration = pulseIn(ECHO, HIGH);   // Reads ECHO, returns the travel time in microseconds

    Serial.print("Distance: "); // debug out
    Serial.print(duration*0.013504/2); // Calculate the distance in inches (13,504 in/s)
    inches = duration*0.013504/2;
    Serial.print(" in."); 
    Serial.println(); // I added this.
    delay(300);                // Loop very quickly
    // - - - - - - - - - - - - - - - - (End)
}

void handleMessage() {
    if (server.hasArg("data")) {
        String data = server.arg("data");
        Serial.println("Received: " + data);
        
        if (data == "redlight") {
            server.send(200, "text/plain", "greenlight");
        }
        // This is the area for sending the state.  
        // Green (go)
        if (inches > 20) { 
            //Serial.println("Green is for Go!"); 
            server.send(200, "text/plain", "green");
        } 
        // Yellow (warning)
        if (inches < 20 && inches > 10) { 
            //Serial.println("Yellow is for Yell!"); 
            server.send(200, "text/plain", "yellow");
        } 
        // Red (stop)
        if (inches < 10 && inches > 5) { 
            //Serial.println("Red is for Run!"); 
            server.send(200, "text/plain", "red");
        } 
        // Too far, back up (fast flashing red)
        if (inches < 5) { 
            //Serial.println("Red is for Run!"); 
            server.send(200, "text/plain", "flashingred");
        }
    } else {
        server.send(400, "text/plain", "No message received");
    }
}

// Note:  what messages does the "stoplight" Arduino need to send to the "distance-sensor"
// one?  The only thing I can think of would be verifying the connection, and that's 
// important for safety reasons (so I'll need to build that in as well.)

// Pinout for the "HC-SR04":
// From front:
// Vcc | Trig | Echo | Gnd
// From back:
// Gnd | Echo | Trig | Vcc

