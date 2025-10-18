
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
    char state = SerialBT.read();
    Serial.print("Received: ");
    Serial.println(state);
    switch(state){
      case '1':
        digitalWrite(LED0,HIGH);
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,LOW);
        break;
      case '2':
        digitalWrite(LED0,LOW);
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,LOW);
        break;
      case '3':
        digitalWrite(LED0,LOW);
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,HIGH);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,LOW);
        break;
      case '4':
        digitalWrite(LED0,LOW);
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,HIGH);
        digitalWrite(LED4,LOW);
        break;
      case '5': 
        digitalWrite(LED0,LOW);
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,HIGH);
        break;
      case '6':
        digitalWrite(LED0,HIGH);
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2,HIGH);
        digitalWrite(LED3,HIGH);
        digitalWrite(LED4,HIGH);
        break;
      default:
        digitalWrite(LED0,LOW);
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,LOW);
        break;
    }

  }

  // Send a periodic message to PC every 2s
  if (millis() - lastSend > 2000) {
    SerialBT.println("Hello from ESP32");
    lastSend = millis();
  }
}
