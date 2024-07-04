#include <TinyGPS++.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define GPS_BAUDRATE 19200  // The default baudrate of NEO-6M is 9600

// Replace with your network credentials
const char* ssid = "";
const char* password = "";

// Replace with your Flask server address
const char* serverName = "http://192.168.0.103:5000/update_location";

TinyGPSPlus gps;  // the TinyGPS++ object

void setup() {
  Serial.begin(9600);

  Serial.println(F("ESP32 - GPS module"));

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void sendGPSData(float latitude, float longitude) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String postData = "{\"latitude\": " + String(latitude, 6) + ", \"longitude\": " + String(longitude, 6) + "}";

    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void loop() {
  delay(5000);
  if (Serial.available() > 0) {
    if (gps.encode(Serial.read())) {
      // Check if location is valid
      if (gps.location.isValid()) {
        float latitude = gps.location.lat();
        float longitude = gps.location.lng();
        
        // Send GPS data to server
        sendGPSData(latitude, longitude);

        // Print GPS data to Serial Monitor
        Serial.print(F("- latitude: "));
        Serial.println(latitude);

        Serial.print(F("- longitude: "));
        Serial.println(longitude);
      } else {
        Serial.println(F("- location: INVALID"));
                sendGPSData(1, 1);

      }

      Serial.println();
    }
  }

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
    sendGPSData(1.70, 103.90);

}
