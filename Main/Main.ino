#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <HttpClient.h>

const char* ssid = "iptime";

int RxPin = 12;
int TxPin = 13;

IPAddress hostIp(00, 000, 00, 00);
int SERVER_PORT = 8080;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RxPin, TxPin);

  Serial.println(String("Connect try : ") + ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }

  Serial.print(String("WIFI conneted\n IP : "));
  Serial.println(WiFi.localIP());
}

void loop() {
  if (Serial2.available()) {
    int value = Serial2.read();
    Serial.println(value);

    HTTPClient http;

    http.begin("http://00.000.00.00:8080/post");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String httpRequestData = "&value=" + String(value);
    Serial.println(httpRequestData);
    int httpResponseCode = http.POST(httpRequestData);

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
    Serial.println("Error in WiFi connection");
  }
  delay(30000000);
}