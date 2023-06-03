#include <WiFi.h>

const char* ssid = "WIFI_NAME";
const char* password = "WIFI_PASSWORD";

const char* apiToken = "API_TOKEN";
const char* userToken = "USER_TOKEN";
const char* pushoverApiEndpoint = "https://api.pushover.net/1/messages.json";
const char *pushoverSLL = "";

unsigned long sinceResponds = 0;
unsigned long respondsInterval = 100; 

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");

  // Wait for connection 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
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
}

void setup() {
  initWiFi();
}

void loop() {
  if (sinceResponds < respondsInterval && WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    delay(1000);
    while (WiFi.status() != WL_CONNECTED) {
      notifyClient("Warning", "Connection lost")
      Serial.println("");
      delay(2000);
    }
  }
  else if (sinceResponds < respondsInterval) {
    sinceResponds = 0;
  }
  else {
    // if moisture
    if (false) {
      notifyClient("Warning", "Moisture detected")
      delay(5000);
    }
    sinceResponds++;
    delay(50);
  }
}
