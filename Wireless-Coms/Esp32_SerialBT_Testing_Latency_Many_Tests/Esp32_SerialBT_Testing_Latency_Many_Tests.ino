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
  SerialBT.setTimeout(200);
  delay(2000);
  bool success = SerialBT.begin("BT_LED_Controller"); // true = master mode
  if (success) {
    Serial.println("Bluetooth device started. Pair and open the COM port.");
  } 
  else {
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
    int size = state.length();
    switch(size){
      case 50: //1 every 10 (10)*5 = actual package size 50
        digitalWrite(LED0,HIGH);
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,LOW);
        break;
      case 100: //2 (20)*5 = actual package size 100
        digitalWrite(LED0,LOW);
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,LOW);
        break;
      case 150: //3 (30)*5 = actual package size 150
        digitalWrite(LED0,HIGH);
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,LOW);
        break;
      case 200: //4 (40)*5 = actual package size 200
        digitalWrite(LED0,LOW);
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,HIGH);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,LOW);
        break;
      case 250: //5 (50)*5 = actual package size 250
        digitalWrite(LED0,HIGH);
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,HIGH);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,LOW);
        break;
      case 300: //6 (60)*5 = actual package size 300 
        digitalWrite(LED0,LOW);
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2,HIGH);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,LOW);
        break;
      case 350: //7 (70)*5 = actual package size 350
        digitalWrite(LED0,HIGH);
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2,HIGH);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,LOW);
        break;
      case 400: //8 (80)*5 = actual package size 400
        digitalWrite(LED0,LOW);
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,HIGH);
        digitalWrite(LED4,LOW);
        break;
      case 450: //9 (90)*5 = actual package size 450
        digitalWrite(LED0,HIGH);
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,HIGH);
        digitalWrite(LED4,LOW);
        break;
      case 500: //10 (100)*5 = actual package size 500
        digitalWrite(LED0,LOW);
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,HIGH);
        digitalWrite(LED4,LOW);
        break;
      case 550: //11 (110)*5 = actual package size 550
        digitalWrite(LED0,HIGH);
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,HIGH);
        digitalWrite(LED4,LOW);
        break;
      case 600: //12 (120)*5 = actual package size 600
        digitalWrite(LED0,LOW);
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,HIGH);
        digitalWrite(LED3,HIGH);
        digitalWrite(LED4,LOW);
        break;
      case 650: //13 (130)*5 = actual package size 650
        digitalWrite(LED0,HIGH);
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,HIGH);
        digitalWrite(LED3,HIGH);
        digitalWrite(LED4,LOW);
        break;
      case 700: //14 (140)*5 = actual package size 700
        digitalWrite(LED0,LOW);
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2,HIGH);
        digitalWrite(LED3,HIGH);
        digitalWrite(LED4,LOW);
        break;
      case 750: //15 (150)*5 = actual package size 750
        digitalWrite(LED0,HIGH);
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2,HIGH);
        digitalWrite(LED3,HIGH);
        digitalWrite(LED4,LOW);
        break;
      case 800: //16 (160)*5 = actual package size 800
        digitalWrite(LED0,LOW);
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,HIGH);
        break;
      case 850: //17 (170)*5 = actual package size 850
        digitalWrite(LED0,HIGH);
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,HIGH);
        break; 
      case 900: //18 (180)*5 = actual package size 900
        digitalWrite(LED0,LOW);
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,HIGH);
        break;
      case 950: //19 (190)*5 = actual package size 950
        digitalWrite(LED0,HIGH);
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,HIGH);
        break;
      case 1000: //20 (200)*5 = actual package size 1000
        digitalWrite(LED0,LOW);
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,HIGH);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,HIGH);
        break;
      case 1050: //21 (210)*5 = actual package size 1050
        digitalWrite(LED0,HIGH);
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,HIGH);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,HIGH);
        break;
      case 1100: //22 (220)*5 = actual package size 1100
        digitalWrite(LED0,LOW);
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2,HIGH);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,HIGH);
        break;
      case 1150: //23 (230)*5 = actual package size 1150
        digitalWrite(LED0,HIGH);
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2,HIGH);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,HIGH);
        break;
      case 1200: //24 (240)*5 = actual package size 1200
        digitalWrite(LED0,LOW);
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,HIGH);
        digitalWrite(LED4,HIGH);
        break;
      case 1250: //25 (250)*5 = actual package size 1250
        digitalWrite(LED0,HIGH);
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,HIGH);
        digitalWrite(LED4,HIGH);
        break;
    }
    if (size > 0){
      //Send Acknowledgement 
      SerialBT.println("ACK");
      SerialBT.flush();
    }
    
  }
}