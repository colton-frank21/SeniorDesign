//On the unity side I had to use keys jkl; and space because of keboard ghosting 
//These keys are in different segments so they can be pressed at the same time
#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "NetworkName";
const char* password = "WIFIPassword";

WiFiUDP udpwifi;
int espPort = 4211;
IPAddress laptopIP(10,0,0,218);
int laptopPort = 4210;
char message[1250]; //Might have to chage this

//Finger Control Outputs
#define thumbPin 1
#define pointerPin 0
#define middlePin 7
#define ringPin 6
#define pinkyPin 5


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  //Wait for connect
  for(int i = 0; i <= 100; i++){
    if(WiFi.status() == WL_CONNECTED){
      break;
    }
    delay(300);
  }
  //could not connect
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Could not connect");
    while(1);
  }

  Serial.println("ESP32 IP = ");
  Serial.println(WiFi.localIP());
  
  udpwifi.begin(espPort); //Setting up "Mailbox" port for ESP32

  //Setup Output Pins
  pinMode(thumbPin, OUTPUT);
  pinMode(pointerPin, OUTPUT);
  pinMode(middlePin, OUTPUT);
  pinMode(ringPin, OUTPUT);
  pinMode(pinkyPin, OUTPUT);
}

void loop() {
  //checking if message is incoming
  int packetsize = udpwifi.parsePacket();
  if(packetsize > 0){
    int messageSize = udpwifi.read(message,1250);
    //making sure message has more than 0 characters
    if (messageSize > 0){
      int thumb   = message[0];
      int pointer = message[1];
      int middle  = message[2];
      int ring    = message[3];
      int pinky   = message[4];
      int checksum= message[5];
      //Check sum 
      int sum = thumb + pointer + middle + ring + pinky;
      if(sum == checksum){
        //send acknowledgement 
        udpwifi.beginPacket(laptopIP, laptopPort);
        udpwifi.write((const uint8_t*)"ACK", 3);
        udpwifi.endPacket();
        //Outputs
        digitalWrite(thumbPin, thumb);
        digitalWrite(pointerPin, pointer);
        digitalWrite(middlePin, middle);
        digitalWrite(ringPin, ring);
        digitalWrite(pinkyPin, pinky);
      }
    }
  }
}
