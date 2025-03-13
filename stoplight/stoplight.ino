  #include <ESP8266WiFi.h>	
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <ESP8266HTTPClient.h>
  #include "wi-fi_config.h"

// Variable to store the device's IP address that responds with "greenlight"
String targetIP = "";

// Function to connect to Wi-Fi
void connectToWiFi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  
  // Try IP addresses in the range 192.168.204.x (1 to 254)
  Serial.print("Scanning for Arduino controlling the distance sensor.");
  for (int i = 1; i <= 254; i++) {
    String ip = "192.168.204." + String(i); // Generate the IP address
    sendHttpRequest(ip);  // Send the HTTP request
    Serial.println();
    Serial.print("Scanning IP:");
    Serial.print(ip);
    if (targetIP != "") {
      break;  // Exit if the target device is found
    }
  }
}

void sendHttpRequest(String ip) {
  WiFiClient client;
  String url = "http://" + ip + "/?redlight"; // Send GET request to the device
  if (client.connect(ip.c_str(), 80)) { // Connect to the device's IP address on port 80
    client.print("GET /?redlight HTTP/1.1\r\n");
    client.print("Host: " + ip + "\r\n");
    client.print("Connection: close\r\n\r\n");
    delay(500); // Wait for response
    String response = client.readString();
    Serial.println();
    Serial.print("This is the response:  ");
    Serial.print(response);
    if (response.indexOf("greenlight") != -1) { // Check if the response is "greenlight"
      targetIP = ip; // Store the IP address of the device
      Serial.print("Found target device at IP: ");
      Serial.println(targetIP);
      client.stop();
      return;
    }
    client.stop();
  }
}

void loop() {
  // Add code here if you need continuous functionality after the device is found
}
