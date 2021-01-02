// This file uses a A4988 Stepper motor driver board 
// The accelStepA4988.h file does a better job than this code and should be used in place of this code. It does have a stop switch and resolves some of the TODOs listed below.

// TODO add at least one limit switch to allow zeroing stepper on startup. 
// Two switches would be good, one for open gate and the other for when the gate is closed
// TODO add a twitch at startup, move the gate a short distance in each direction to show it is working
// This code is written so steps are made asyncroniously to as not to trigger the built in watchdog in the ESP8266
/*
This is just to drive the acme thread forward and back 4"
If stepper runs, "gGateStatus " is toggled
10/14/20
Redone to work with Wemos D1 mini and added to the Blast Gate code
11/6/2020 GHA
*/
#define ENABLE_PIN D1      //pin to turn off the stepper board on and off
#define DIRECTION_PIN D2   //Wemos pin connected to direction pin on stepper board
#define STEP_PIN  D3       //Wemos pin connected Step signal pin on stepper board

const int powerSensorPin =D4; //pin for switch to trigger open or close
//long loopCount =0;

typedef enum
  {
    CW,         // 0
    CCW,        // 1
  }  StepDirection;
  StepDirection _direction;
  bool _stopped =true;
  unsigned int _currentStep =0;
  
  unsigned int _goalStep = 0;
  unsigned long _startime = 0;
  const int gStepsPerRevolution = 200; // change as needed for your stepper
  const float gStepperSpeedRpm = 250;  //increase to speed up to 400 works
  unsigned int gStepsToOpenGate = 2700;
  unsigned long gMicrosPerStep;
void AStepperSteps(unsigned int stepsToMove, StepDirection direction)
{
  _stopped = false;
  _direction = direction;
  _goalStep = stepsToMove;
  _startime = micros();
  digitalWrite(DIRECTION_PIN,direction);
  digitalWrite(ENABLE_PIN, LOW);//low turns the board on
}
void openBlastGate(){
  //  if(digitalRead(openedLimitPin) ){ //check limit switch to see if the gate is already open. If not open it.
   AStepperSteps(gStepsToOpenGate, CCW);
  gGateStatus  = !gGateStatus ; 
  //}else{ //gate already open
//  }
 }
  void closeBlastGate(){
//  if(digitalRead(closedLimitPin) ){//check limit switch to see if the gate is already closed. If not close it.
//    //add code to close the gate
   AStepperSteps(gStepsToOpenGate, CW);
   gGateStatus  = !gGateStatus ;
  //  }else{// gate already open
//  }
}
 void AStepperSetSpeedRpm(float rpm)
{
 float degreesBySecond = (rpm * 360 / 60);
 float degreeByStep = 360.0f / gStepsPerRevolution;
 gMicrosPerStep = degreeByStep / degreesBySecond * 1000000;
 Serial.print("gMicrosPerStep = ");
 Serial.println(gMicrosPerStep);
} 
bool AStepperUpdate(){
  if (_stopped) {
  return false;
  }
  if (static_cast<unsigned long>(micros() - _startime) >= gMicrosPerStep ){
    _startime += gMicrosPerStep ;
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(50);
    digitalWrite(STEP_PIN, LOW);
    _goalStep--;
    if (_goalStep == 0){
        _stopped = true;
        digitalWrite(ENABLE_PIN, HIGH);//high turns the board off
        Serial.println("");
        Serial.println("Stepper done moving");
      }
    }
    return true;
  }


void gateSetup(){
  pinMode(ENABLE_PIN, OUTPUT);      //Sets enable pin as an output
  pinMode(DIRECTION_PIN , OUTPUT); 
  pinMode(STEP_PIN , OUTPUT); 
  pinMode(powerSensorPin , INPUT_PULLUP);
  digitalWrite(ENABLE_PIN, LOW);    //Low enables the driver
  AStepperSetSpeedRpm(gStepperSpeedRpm);
  //TODO move the stepper back and forth to show that it is working
  // and/or move the gate to the closed position.
}
void gateLoop(){
  AStepperUpdate();
}
