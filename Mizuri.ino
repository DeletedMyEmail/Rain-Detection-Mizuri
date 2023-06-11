#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "wifi name";
const char* password = "wifi password";

const char* apiToken = "yourToken";
const char* userToken = "yourToken";
const char *PUSHOVER_API_URL = "https://api.pushover.net/1/messages.json";
const char *PUSHOVER_ROOT_CA = "-----BEGIN CERTIFICATE-----\n"
                 "MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n"
                 "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
                 "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n"
                 "QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n"
                 "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
                 "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n"
                 "9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n"
                 "CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n"
                 "nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n"
                 "43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n"
                 "T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n"
                 "gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n"
                 "BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n"
                 "TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n"
                 "DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n"
                 "hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n"
                 "06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n"
                 "PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n"
                 "YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n"
                 "CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n"
                 "-----END CERTIFICATE-----\n";

unsigned long sinceResponds = 0;
unsigned long respondsInterval = 100; 

void setup() {
  Serial.begin(115200);
  Serial.println("Guten Morgen");
  initWiFi();
  notifyClient("Info", "Moisture Sensor Mizuri Online!");
}

void loop() {
  Serial.println("Loop");
  if (sinceResponds < respondsInterval && WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    delay(1000);
    while (WiFi.status() != WL_CONNECTED) {
      notifyClient("Warning", "Connection lost");
      Serial.println("Connection lost");
      delay(10000);
    }
  }
  else if (sinceResponds < respondsInterval) {
    sinceResponds = 0;
  }
  else {
    // if moisture
    if (false) {
      Serial.println("Moisture detected");
      notifyClient("Warning", "Moisture detected");
      delay(10000);
    }
    sinceResponds++;
    delay(50);
  }
  delay(3000);
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");

  // Wait for connection 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(WiFi.status());
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void notifyClient(const char* title, const char* message) {
    StaticJsonDocument<512> notification; 
    notification["token"] = apiToken; 
    notification["user"] = userToken;
    notification["message"] = message;
    notification["title"] = title;

    String jsonStringNotification;
    serializeJson(notification, jsonStringNotification);

    HTTPClient https;
    https.begin(PUSHOVER_API_URL , PUSHOVER_ROOT_CA);
    https.addHeader("Content-Type", "application/json");

    int httpResponseCode = https.POST(jsonStringNotification);
    Serial.println(httpResponseCode);
}
