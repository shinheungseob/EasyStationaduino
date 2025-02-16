#include <ArduinoJson.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
const int realaypin = 25;
WiFiMulti wifiMulti;
void setup() {
    pinMode(realaypin, OUTPUT);
    Serial.begin(115200);
    wifiMulti.addAP("codegear", "qwertyuiop");
}
void loop() {
    if ((wifiMulti.run() == WL_CONNECTED)) {
        HTTPClient http;
        Serial.print("[HTTP] begin...\n");
        http.begin("https://heung0427.mycafe24.com/iot_update.php?device=1");
        Serial.print("[HTTP] GET...\n");
        int httpCode = http.GET();
        if (httpCode > 0) {
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);
            if (httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.println("Received payload: " + payload);
                // JSON 디코드를 위한 JsonDocument 생성
                DynamicJsonDocument doc(1024);
                // JSON 데이터 파싱
                DeserializationError error = deserializeJson(doc, payload);
                // JSON 파싱이 성공하면
                if (!error) {
                    // "status" 키의 값을 확인하고 처리
                    int status = doc["light"];
                    if (status == 0) {
                        digitalWrite(realaypin, LOW);
                        Serial.println("Setting relay to LOW");
                    } else if (status == 1) {
                        digitalWrite(realaypin, HIGH);
                        Serial.println("Setting relay to HIGH");
                    } else {
                        Serial.println("Invalid status value");
                    }
                } else {
                    Serial.println("Failed to parse JSON");
                }
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
    delay(2000);
}
```
