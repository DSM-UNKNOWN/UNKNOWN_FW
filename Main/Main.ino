#include <WiFi.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <HTTPClient.h>

const char* ssid = "2316";
const char* password = "";

TinyGPSPlus gps;
SoftwareSerial gpsSerial(12, 13);

const char* serverUrl = "http://000.000.0.00:8080/user";

void setup() {
  Serial.begin(115200); 
  gpsSerial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.println("IP address: " + WiFi.localIP().toString());
}

void loop() {
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);

    if (gps.location.isUpdated()) {
      double latitude = gps.location.lat();
      double longitude = gps.location.lng();
      Serial.printf("Latitude: %.6f, Longitude: %.6f\n", latitude, longitude);

      sendGpsData(latitude, longitude);
    }
  }
}

void sendGpsData(double latitude, double longitude) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    String jsonPayload = String("{\"latitude\":") + latitude + ",\"longitude\":" + longitude + "}";

    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Server Response: " + response);
    } else {
      Serial.println("Error on sending POST: " + String(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected. Cannot send data.");
  }
}
