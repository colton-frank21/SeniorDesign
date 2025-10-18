#include <WiFi.h>

#include <WiFiUdp.h>

const char * ssid = "NetworkName";
const char * password = "Password";

WiFiUDP udpwifi;
int espPort = 4211;
//Esp IP on my network 10.0.0.6
IPAddress laptopIP(10, 0, 0, 218);
int laptopPort = 4210;
char message[1250]; //Might have to chage this

//Finger Control Outputs
#define LED0 1
#define LED1 0
#define LED2 7
#define LED3 6
#define LED4 5

//message count
int count = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  //Wait for connect
  for (int i = 0; i <= 100; i++) {
    if (WiFi.status() == WL_CONNECTED) {
      break;
    }
    delay(300);
  }
  //could not connect
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Could not connect");
    while (1);
  }

  Serial.println("ESP32 IP = ");
  Serial.println(WiFi.localIP());

  udpwifi.begin(espPort); //Setting up "Mailbox" port for ESP32

  //Setup Output Pins
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
}

void loop() {
  //checking if message is incoming
  int packetsize = udpwifi.parsePacket();
  if (packetsize > 0) {
    int messageSize = udpwifi.read(message, 1250);
    //making sure message has more than 0 characters
    if (messageSize > 0) {
        count++;
        char ackmessage[7];
        sprintf(ackmessage, "ACK%d", count);

        udpwifi.beginPacket(laptopIP, laptopPort);
        udpwifi.write((const uint8_t * )ackmessage, strlen(ackmessage));
        udpwifi.endPacket();
        //Outputs
        
        switch (count) {
        case 10: //1 every 10 (10)*5 = actual package size 50
          digitalWrite(LED0, HIGH);
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, LOW);
          break;
        case 20: //2 (20)*5 = actual package size 100
          digitalWrite(LED0, LOW);
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, LOW);
          break;
        case 30: //3 (30)*5 = actual package size 150
          digitalWrite(LED0, HIGH);
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, LOW);
          break;
        case 40: //4 (40)*5 = actual package size 200
          digitalWrite(LED0, LOW);
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, LOW);
          break;
        case 50: //5 (50)*5 = actual package size 250
          digitalWrite(LED0, HIGH);
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, LOW);
          break;
        case 60: //6 (60)*5 = actual package size 300 
          digitalWrite(LED0, LOW);
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, LOW);
          break;
        case 70: //7 (70)*5 = actual package size 350
          digitalWrite(LED0, HIGH);
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, LOW);
          break;
        case 80: //8 (80)*5 = actual package size 400
          digitalWrite(LED0, LOW);
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, HIGH);
          digitalWrite(LED4, LOW);
          break;
        case 90: //9 (90)*5 = actual package size 450
          digitalWrite(LED0, HIGH);
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, HIGH);
          digitalWrite(LED4, LOW);
          break;
        case 100: //10 (100)*5 = actual package size 500
          digitalWrite(LED0, LOW);
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, HIGH);
          digitalWrite(LED4, LOW);
          break;
        case 110: //11 (110)*5 = actual package size 550
          digitalWrite(LED0, HIGH);
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, HIGH);
          digitalWrite(LED4, LOW);
          break;
        case 120: //12 (120)*5 = actual package size 600
          digitalWrite(LED0, LOW);
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, HIGH);
          digitalWrite(LED4, LOW);
          break;
        case 130: //13 (130)*5 = actual package size 650
          digitalWrite(LED0, HIGH);
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, HIGH);
          digitalWrite(LED4, LOW);
          break;
        case 140: //14 (140)*5 = actual package size 700
          digitalWrite(LED0, LOW);
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, HIGH);
          digitalWrite(LED4, LOW);
          break;
        case 150: //15 (150)*5 = actual package size 750
          digitalWrite(LED0, HIGH);
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, HIGH);
          digitalWrite(LED4, LOW);
          break;
        case 160: //16 (160)*5 = actual package size 800
          digitalWrite(LED0, LOW);
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, HIGH);
          break;
        case 170: //17 (170)*5 = actual package size 850
          digitalWrite(LED0, HIGH);
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, HIGH);
          break;
        case 180: //18 (180)*5 = actual package size 900
          digitalWrite(LED0, LOW);
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, HIGH);
          break;
        case 190: //19 (190)*5 = actual package size 950
          digitalWrite(LED0, HIGH);
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, HIGH);
          break;
        case 200: //20 (200)*5 = actual package size 1000
          digitalWrite(LED0, LOW);
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, HIGH);
          break;
        case 210: //21 (210)*5 = actual package size 1050
          digitalWrite(LED0, HIGH);
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, HIGH);
          break;
        case 220: //22 (220)*5 = actual package size 1100
          digitalWrite(LED0, LOW);
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, HIGH);
          break;
        case 230: //23 (230)*5 = actual package size 1150
          digitalWrite(LED0, HIGH);
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, HIGH);
          break;
        case 240: //24 (240)*5 = actual package size 1200
          digitalWrite(LED0, LOW);
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, HIGH);
          digitalWrite(LED4, HIGH);
          break;
        case 250: //25 (250)*5 = actual package size 1250
          digitalWrite(LED0, HIGH);
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, HIGH);
          digitalWrite(LED4, HIGH);
          count = 0;
          break;
      }
    }
  }
}