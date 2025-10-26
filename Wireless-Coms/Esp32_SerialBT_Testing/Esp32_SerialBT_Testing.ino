
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

#define LED0 27
#define LED1 26
#define LED2 25
#define LED3 33
#define LED4 32 

unsigned long lastSend = 0;

void setup() {
  Serial.begin(115200);               // USB debug
  delay(2000);
  bool success = SerialBT.begin("BT_LED_Controller"); // true = master mode
if (success) {
  Serial.println("Bluetooth device started. Pair and open the COM port.");
} else {
  Serial.println("Bluetooth failed to start!");
}
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  Serial.println("Bluetooth device started. Pair and open the COM port.");
}

void loop() {
  // Receive from PC
  if (SerialBT.available()) {
    String state = SerialBT.readStringUntil('\n');
    Serial.print("Received: ");
    Serial.println(state);
    if(state.length() > 0){
      int thumb = state[0] - 0;
      int pointer = state[1] - 0;
      int middle = state[2] - 0;
      int ring = state[3] - 0;
      int pinky = state[4] - 0;
      if(thumb == 1){ digitalWrite(LED0, HIGH); }
      else{ digitalWrite(LED0, LOW); }
      if(pointer == 1){ digitalWrite(LED1, HIGH); }
      else{ digitalWrite(LED1, LOW); }
      if(middle == 1){ digitalWrite(LED2, HIGH); }
      else{ digitalWrite(LED2, LOW); }
      if(ring == 1){ digitalWrite(LED3, HIGH); }
      else{ digitalWrite(LED3, LOW); }
      if(pinky == 1){ digitalWrite(LED4, HIGH); }
      else{ digitalWrite(LED4, LOW); }
    }
  }

  // Send a periodic message to PC every 2s
  if (millis() - lastSend > 2000) {
    SerialBT.println("Hello from ESP32");
    lastSend = millis();
  }
}
