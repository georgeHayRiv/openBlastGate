// This file uses a 28BYJ-48 5VDC Stepper motor that is geared 1/64 so turns very slowly
// This is not a practical way to open a blast gate but was done while prototyping using a stepper.
// There are power issues with this board and it will only work if the Wemos D1 is powered up and then the stepper shield is attached. Same goes for uploading a sketch to the Wemos. The stepper board needs to be detached.

// TODO add at least one limit switch to allow zeroing stepper on startup. 
// Two switches would be good, one for open gate and the other for when the gate is closed
// TODO add a twitch at startup, move the gate a short distance in each direction to show it is working
// This code is written so steps are made asyncroniously so as to as not to trigger the built in watchdog in the ESP8266
#define GATE_H "gateStepper.h"

#include "AsyncStepperLib.h"
#include <Stepper.h>
//const int powerSensorPin = D1;
const int motorPin1 = D5;  
const int motorPin2 = D6;  
const int motorPin3 = D7; 
const int motorPin4 = D8; 
const int numSteps = 8;
const int stepsLookup[8] = { B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001 };
int stepCounter = 0; 
void setOutput(int step){
  digitalWrite(motorPin1, bitRead(stepsLookup[step], 0));
  digitalWrite(motorPin2, bitRead(stepsLookup[step], 1));
  digitalWrite(motorPin3, bitRead(stepsLookup[step], 2));
  digitalWrite(motorPin4, bitRead(stepsLookup[step], 3));
}
void clockwise(){
  stepCounter++;
  if (stepCounter >= numSteps) stepCounter = 0;
  setOutput(stepCounter);
}
void anticlockwise(){
  stepCounter--;
  if (stepCounter < 0) stepCounter = numSteps - 1;
  setOutput(stepCounter);
}
const int stepsPerRevolution = 4076;
AsyncStepper stepper1(stepsPerRevolution,
  []() {clockwise(); },
  []() {anticlockwise(); }
);
void openBlastGate(){
//  if(digitalRead(openedLimitPin) ){ //check limit switch to see if the gate is already open. If not open it.
    //add code to open the gate
    stepper1.RotateToAngle(360*4,AsyncStepper::CCW);
    gGateStatus = GATE_OPEN;
//  }else{ //gate already open
//  }
 }
  void closeBlastGate(){
//  if(digitalRead(closedLimitPin) ){//check limit switch to see if the gate is already closed. If not close it.
//    //add code to close the gate
      stepper1.RotateToAngle(360*4,AsyncStepper::CW);
      gGateStatus = GATE_CLOSED;
//  }else{// gate already open
//  }
}
void turnOffStepper(){
  stepper1.Stop();
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}
void gateSetup(){
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  stepper1.SetSpeedRpm(10);// nothing faster than 10 RPM seems to work with small greared stepper
  turnOffStepper();
}
void gateLoop(){
  stepper1.Update();
}
