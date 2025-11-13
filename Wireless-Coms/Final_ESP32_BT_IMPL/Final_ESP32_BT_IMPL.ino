#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

#define LED0 27
#define LED1 26
#define LED2 25
#define LED3 33
#define LED4 32
#define Kill_Switch 13

//Hardware Level Interrupt Handler
void IRAM_ATTR killSwitchInter();

unsigned long lastSend = 0;
bool killState = false;

void setup() {
  Serial.begin(115200); // USB debug
  delay(2000);
  bool success = SerialBT.begin("BT_LED_Controller_SD"); // true = master mode
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

  //HW Interrupt Setup
  pinMode(Kill_Switch, INPUT);
  attachInterrupt(digitalPinToInterrupt(Kill_Switch), killSwitchInter, HIGH);
}

void loop() {
  if (killState == true) {
    digitalWrite(LED0, LOW);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    while (1) {
      delay(1000);
    }
  }

  // Receive from PC
  if (SerialBT.available()) {
    String state = SerialBT.readStringUntil('\n');
    if (state.length() > 0) {
      int thumb = (uint8_t)state[0];
      int pointer = (uint8_t)state[1];
      int middle = (uint8_t)state[2];
      int ring = (uint8_t)state[3];
      int pinky = (uint8_t)state[4]; 
      //Error Checking
      int checksum = (uint8_t)state[5];
      int sum = thumb + pointer + middle + ring + pinky;
      if(checksum == sum){
        if (thumb == 1) {
          digitalWrite(LED0, HIGH);
        } else {
          digitalWrite(LED0, LOW);
        }
        if (pointer == 1) {
          digitalWrite(LED1, HIGH);
        } else {
          digitalWrite(LED1, LOW);
        }
        if (middle == 1) {
          digitalWrite(LED2, HIGH);
        } else {
          digitalWrite(LED2, LOW);
        }
        if (ring == 1) {
          digitalWrite(LED3, HIGH);
        } else {
          digitalWrite(LED3, LOW);
        }
        if (pinky == 1) {
          digitalWrite(LED4, HIGH);
        } else {
          digitalWrite(LED4, LOW);
        }

        //ACK Section
        int p4 = (uint8_t)state[6];
        SerialBT.println(p4);
      }
      else{
        //The Checksum did not match
        SerialBT.println("ERR");
      }
    }
  }

  // Send a periodic message to PC every 2s
  if (millis() - lastSend > 2000) {
    SerialBT.println("Hello from ESP32");
    lastSend = millis();
  }
}

void IRAM_ATTR killSwitchInter() {
  killState = true;
}