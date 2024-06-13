#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "SSID";
const char* password = "PASSWORD";
const String url = "http://192.xxx.xx.x:5000/sensors";

const int sensorPin = 34;  // Pin analog untuk sensor kelembaban tanah

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
}

void loop() {
  // Baca nilai sensor kelembaban tanah
  int moistureValue = analogRead(sensorPin);

  // Buat objek JSON untuk data yang akan dikirim
  StaticJsonDocument<200> doc;
  doc["moisture"] = moistureValue;
  doc["time"] = millis();
  String requestBody;
  serializeJson(doc, requestBody);

  // Tampilkan data yang akan dikirim ke Serial Monitor
  Serial.print("Sending data: ");
  Serial.println(requestBody);

  // Kirim data ke server jika terhubung ke WiFi
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    // Kirim HTTP POST request
    int httpResponseCode = http.POST(requestBody);

    // Tampilkan respons dari server
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.print("Response: ");
      Serial.println(response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    // Akhiri koneksi HTTP
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(5000); // Delay 5 detik sebelum mengirim data lagi
}
