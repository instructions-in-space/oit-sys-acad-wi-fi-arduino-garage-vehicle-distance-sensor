  #include <ESP8266WiFi.h>	
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <ESP8266HTTPClient.h>
  #include "wi-fi_config.h"

  // Define pins for LEDs
	const int redLED = 16;
	const int yellowLED = 14;
	const int greenLED = 12;

  // Create a server to listen on port 80
  WiFiServer server(80);

  // Starting IP address (you can tweak these values based on your network's configuration)
  String baseIP = "192.168.204.";  // The subnet of your network
  int startRange = 2;   // Starting IP address range (Usually 192.168.1.2 onwards)
  int endRange = 255;   // Ending IP address range (You can limit this for faster scanning)

	void setup() {
     // Setup serial port
    Serial.begin(115200);
    Serial.println();

    // Begin Wi-Fi 
    WiFi.begin(WIFI_SSID,WIFI_PASS);
    // I may not want this section after all, but I'll keep it here for now.
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());  // Print the IP address assigned by DHCP

    // Start scanning local network
    scanNetwork();

    // Start the server
    server.begin();

	  // Set each LED pin as an OUTPUT
	  pinMode(redLED, OUTPUT);
	  pinMode(yellowLED, OUTPUT);
	  pinMode(greenLED, OUTPUT);
	}

  void scanNetwork() {
    // Try GET request to each IP in the local subnet
    for (int i = startRange; i <= endRange; i++) {
      String ip = baseIP + String(i);
      sendGETRequest(ip);
      delay(500);  // Add delay to avoid flooding the network
    }
  }

  void sendGETRequest(String ip) {
    String url = "http://" + ip + "/request";  // Customize this URL based on your device's expected endpoint
  
  // Set up HTTP Client
  HTTPClient http;
  begin(WiFiClient, url);
  int httpCode = http.GET();
  
  // Check if the HTTP request was successful
  if (httpCode > 0) {
    Serial.print("HTTP GET request to ");
    Serial.print(ip);
    Serial.print(" successful. Response code: ");
    Serial.println(httpCode);

    String payload = http.getString();
    Serial.print("Response: ");
    Serial.println(payload);
    
    // You can add custom logic to identify the correct device based on the response payload
    if (payload.indexOf("YourUniqueDeviceIdentifier") >= 0) {
      Serial.println("Target device found!");
    }
  } else {
    Serial.print("HTTP GET request failed for ");
    Serial.print(ip);
    Serial.print(". Error code: ");
    Serial.println(httpCode);
  }

  http.end();
}





	void loop() {
    WiFiClient client = server.available();  // Listen for incoming clients

    if (client) {
      // Serial.println("New Client");
      // Wait until the client sends some data
      while (client.connected()) {
        if (client.available()) {
          String raw_command = client.readStringUntil('\n');
          client.flush();
          const char* web_command = raw_command.c_str();  // Converts true string into char array (C string)
          //Serial.print(web_command);
          //Serial.println();

          // -----------------------------------------------------------------------------------
          // Automatic mode off (all lights turn off)
          if(strstr(web_command, "automaticoff")) {
            //Serial.println();
	          // All lights OFF
	          digitalWrite(redLED, LOW);
	          digitalWrite(yellowLED, LOW);
	          digitalWrite(greenLED, LOW);
            Serial.print("All lights off");
            Serial.println();
          }

          // -----------------------------------------------------------------------------------
          // *** Manual Modes ***

          // Manual mode, red light on
          if(strstr(web_command, "redon")) {
            //Serial.println();
	          // Red light ON
	          digitalWrite(redLED, HIGH);
            Serial.print("Red light on");
            Serial.println();
          }

          // Manual mode, red light off
          if(strstr(web_command, "redoff")) {
            //Serial.println();
	          // Red light OFF
	          digitalWrite(redLED, LOW);
            Serial.print("Red light off");
            Serial.println();
          }

          // Manual mode, yellow light on
          if(strstr(web_command, "yellowon")) {
            //Serial.println();
	          // Yellow light ON
	          digitalWrite(yellowLED, HIGH);
            Serial.print("Yellow light on");
            Serial.println();
          }

          // Manual mode, yellow light off
          if(strstr(web_command, "yellowoff")) {
            //Serial.println();
	          // Yellow light OFF
	          digitalWrite(yellowLED, LOW);
            Serial.print("Yellow light off");
            Serial.println();
          }

          // Manual mode, green light on
          if(strstr(web_command, "greenon")) {
            //Serial.println();
	          // Green light ON
	          digitalWrite(greenLED, HIGH);
            Serial.print("Green light on");
            Serial.println();
          }

          // Manual mode, green light off
          if(strstr(web_command, "greenoff")) {
            //Serial.println();
	          // Green light OFF
	          digitalWrite(greenLED, LOW);
            Serial.print("Green light off");
            Serial.println();
          }
          // -----------------------------------------------------------------------------------
          // Send a basic response back to the client
          client.print("HTTP/1.1 200 OK\r\n");
          //client.print("Content-Type: text/html\r\n\r\n");
          //client.print("<html><body><h1>Hello, Wemos D1 Mini!</h1></body></html>");
          break;
        }
      }
    }
	}

