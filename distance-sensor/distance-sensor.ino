#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include "wi-fi_config.h"

// Create an instance of the server (on port 80)
ESP8266WebServer server(80);

String lastHostIP = "";  // Variable to store the IP of the other host

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");

  // Print the local IP address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Define the GET request handler
  server.on("/", HTTP_GET, handleRequest);

  // Start the server
  server.begin();
}

void loop() {
  // Handle incoming HTTP requests
  server.handleClient();
}

void handleRequest() {
  // Get the request URL
  String request = server.uri();
  
  // Get the IP address of the requesting host
  lastHostIP = server.client().remoteIP().toString();
  Serial.print("Request received from IP: ");
  Serial.println(lastHostIP);

  // Check if the request contains "redlight"
  if (request.indexOf("redlight") != -1) {
    // If so, send a GET request to the other host with "greenlight"
    String greenlightURL = "http://" + lastHostIP + "/?greenlight";
    sendGreenLightRequest(greenlightURL);

    // Respond back with a confirmation message
    server.send(200, "text/plain", "Received redlight, sent greenlight request to " + lastHostIP);
    Serial.print("Received 'redlight' message; responded with 'greenlight'.");
    Serial.print(lastHostIP);
  } else {
    // If the request doesn't contain "redlight", respond with a different message
    server.send(200, "text/plain", "Request received but doesn't contain 'redlight'");
    server.send(200, "text/plain", request);
    Serial.print(request);
  }
}

void sendGreenLightRequest(String url) {
  // Use HTTPClient to send a GET request to the other host
  WiFiClient client;
  HTTPClient http;

  // Send the GET request
  http.begin(client, url);  // URL to send the GET request to
  int httpCode = http.GET(); // Send the GET request

  // Print the response code
  if (httpCode > 0) {
    Serial.print("HTTP GET Response code: ");
    Serial.println(httpCode);
  } else {
    Serial.print("HTTP GET request failed, error: ");
    Serial.println(http.errorToString(httpCode).c_str());
  }

  // Close the connection
  http.end();
}

