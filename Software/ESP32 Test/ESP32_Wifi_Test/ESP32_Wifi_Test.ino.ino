#include <WiFi.h>

const char* ssid = "TumharaWiFiName";      // Apna WiFi naam likho
const char* password = "TumharaPassword";   // Apna WiFi password

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

}
