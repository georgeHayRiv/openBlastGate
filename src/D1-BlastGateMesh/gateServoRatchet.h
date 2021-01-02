// TODO do we need to close the gate at startup or do some sort of check to determine where the servo is?

// This code is designed to turn a disc that looks much like a 10" saw blade with 16 teeth. It takes four swings back and forth of the servo to move from the open to closed position.
//This allows a weaker servo to open of close a gate but does lose the ability to know if the gate is open or closed based only on the position of the servo.

#include "Servo.h"  // include Servo library
#define SERVO_PIN  D6
#define OPEN_ANGLE 10 //These angles need to be changed based on the physical setup on the blast gate
#define CLOSED_ANGLE 125 //
Servo gateServo; //initialize a servo object
const int gSwingsPerQuarterRev = 8;
//const int delayTillOff = 2000; //millis before the servo is turned off. Done to save power and stop jittering of the servo.
const int delayTillNext = 1000;
int gSwings = -1;//-1 will make nextCog(); not take any action at startup
void detachServo(); //Prototype so compiler doesn't complain
void nextCog();
//TaskScheduler is included by painlessMesh and tasks are checked in the loop() by painlessMesh
Task taskNextCog( delayTillNext ,TASK_FOREVER, &nextCog );

void detachServo(){
  gateServo.detach();
}
void nextCog(){
   Serial.print("gSwings = ");
    Serial.println(gSwings);
  if(gSwings == 0){
    detachServo();
    gSwings--;
  }
  if(gSwings > 0){
   
    gSwings--;
    gateServo.attach(SERVO_PIN);
    if(gSwings % 2 == 1){ //odd move forward
      gateServo.write(OPEN_ANGLE);
    }else{// move back
      gateServo.write(CLOSED_ANGLE);
    }
  }
}
void turn90Degrees(){
  gateServo.attach(SERVO_PIN);
  gSwings = gSwingsPerQuarterRev;//start moving servo 4 notches forward
}
void openBlastGate(){
  turn90Degrees();
  Serial.println("openBlastGate");
}
  void closeBlastGate(){
    turn90Degrees();
   Serial.println("closeBlastGate");
}

void gateSetup(){
  //gateServo.attach(SERVO_PIN); // connect the servo to the output pin
  userScheduler.addTask( taskNextCog );
  taskNextCog.enable();
  //delay(250);
  //closeBlastGate(); // move back to the closed position
  //TODO instead of the closeBlastGate above there should be code to move until a stop switch is triggered signalling the gate is in the closed position.
}
void gateLoop(){
  //nothing to do in the loop for the servo
  // the taskNextCog task is handled by painlessMesh
}
