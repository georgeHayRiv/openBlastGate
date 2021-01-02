// Use this header file when a gate is being opened and closed using a servo.

#include "Servo.h"  // include Servo library
#define SERVO_PIN  D6
#define OPEN_ANGLE 10 //These angles need to be changed based on the physical setup on the blast gate
#define CLOSED_ANGLE 135 //
Servo gateServo; //initialize a servo object
const int delayTillOff = 2000; //millis before the servo is turned off. Done to save power and stop jittering of the servo.
void detachServo();
//TaskScheduler is included by painlessMesh and tasks are checked in the loop() by painlessMesh
Task taskDetachServo( delayTillOff , 1, &detachServo );//turn off the servo motor after 4 seconds
void detachServo(){
  gateServo.detach();
}
void openBlastGate(){
  gateServo.attach(SERVO_PIN);
  gateServo.write(OPEN_ANGLE);
   taskDetachServo.setIterations(1);
  taskDetachServo.enableDelayed(delayTillOff);
}
  void closeBlastGate(){
   gateServo.attach(SERVO_PIN);
  gateServo.write(CLOSED_ANGLE);
  taskDetachServo.setIterations(1);
  taskDetachServo.enableDelayed(delayTillOff);
  
}
void gateSetup(){
  gateServo.attach(SERVO_PIN); // connect the servo to the output pin
  gateServo.write(CLOSED_ANGLE -10);//move the servo arm 10 degrees (twitch the servo to show it is working)
  userScheduler.addTask( taskDetachServo );
  delay(250);
  closeBlastGate(); // move back to the closed position

}
void gateLoop(){
  //nothing to do in the loop for the servo
  // the taskDetachServo task is handled by painlessMesh
}
