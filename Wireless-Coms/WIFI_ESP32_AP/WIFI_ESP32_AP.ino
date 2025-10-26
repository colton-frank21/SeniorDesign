#include <wifi.h>

#define ssid "ESP32AP"
#define password "Senior_Design"

WiFiserver server(80);

void setup() {
  Serial.begin(115200);
  
  WiFi.softAP(ssid, password);

  IPAddress ESPIP = WiFi.softAPIP()
}

void loop() {
  // put your main code here, to run repeatedly:

}
