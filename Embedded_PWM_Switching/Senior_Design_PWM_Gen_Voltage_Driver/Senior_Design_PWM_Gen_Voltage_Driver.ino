#include <cmath>
#include <string>
#include <vector>

#define PWM_Pin 40 


#define Duty_Pot_Pin 1 //Adjust to change the duty cycle
//These are the button pins starting from bottom of bread board and moving up
#define B1 4
#define B2 6
#define B3 15
#define B4 17
#define B5 8
#define B6 46 //This is the reset pin for the leds

//Led Outputs
#define L1 37 
#define L2 35
#define L3 45
#define L4 47
#define L5 20

//Vectors for storing button and led info
std::vector<int> Buttons = {B1,B2,B3,B4,B5,B6};
std::vector<int> Outputs = {L1,L2,L3,L4,L5};
std::vector<int> States = {0,0,0,0,0};


int Max_Freq = 1000;
int Resolution = 8;
int Max_Duty = pow(2, Resolution) - 1;

//Button Toggle function 
void buttonToggle(std::vector<int>& states, std::vector<int> buttons, std::vector<int> outputs);


void setup() {
  Serial.begin(115200);
  pinMode(Duty_Pot_Pin, INPUT); //Setting up the Potentiometer Input of the Duty Cycle Control
  
 
  ledcAttach(PWM_Pin, Max_Freq, Resolution);   //Setting Up the PWM Output Pin
  ledcWrite(PWM_Pin, Max_Duty/2);              //Setting the default Duty cycle to 50%

  //Setting up Button inputs
  pinMode(B1,INPUT);
  pinMode(B2,INPUT);
  pinMode(B3,INPUT);
  pinMode(B4,INPUT);
  pinMode(B5,INPUT);
  pinMode(B6,INPUT);

  //Setting Up LED outputs
  pinMode(L1,OUTPUT);
  pinMode(L2,OUTPUT);
  pinMode(L3,OUTPUT);
  pinMode(L4,OUTPUT);
  pinMode(L5,OUTPUT);

}

void loop() {
  //Reading Values from Potentiometer Inputs (Anlog Value 0-4095)
  int DutyPotVal = analogRead(Duty_Pot_Pin);
  //Percentage Calculations for Duty Cycle and Frequency(Could have just divided the analog signal by 4095 for same effect)
  float Duty_Percent = map(DutyPotVal, 0 , 4095, 0, 100)/100.0;
  //Duty Cylce for Waveform
  int Duty = Duty_Percent * Max_Duty;
  //Changing Duty Cycle and Frequency
  ledcWrite(PWM_Pin, Duty);
  //Getting Duty Cycle of the waveform, and printing to terminal
  int SquareWaveDuty = ledcRead(PWM_Pin);
  Serial.println("Duty Cyle of Square Wave is: " + String(SquareWaveDuty));

  buttonToggle(States,Buttons,Outputs);

  delay(200);
}

void buttonToggle(std::vector<int>& states, std::vector<int> buttons, std::vector<int> outputs){
  if(digitalRead(buttons[5]) == HIGH){
    states.clear();
    states = {0,0,0,0,0};
    for(int i = 0; i < buttons.size() - 1; i++){
      digitalWrite(outputs[i],states[i]);
    }
  }
  
  for(int i = 0; i < buttons.size() - 1; i++){
    if(digitalRead(buttons[i]) == HIGH){
      states[i] ^= 1;
      Serial.println("The current state of button" + String(states[i]));
      digitalWrite(outputs[i],states[i]);
      delay(50);
    }
  }
}
