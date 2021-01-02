// Include the AccelStepper library:
#include <AccelStepper.h>
// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
unsigned int gOpenGatePosition = -8800;//2700;
unsigned int gClosedGatePosition = -50;
#define ENABLE_PIN D6      //pin to turn off the stepper board on and off
#define STEP_PIN  D7       //Wemos pin connected Step signal pin on stepper board
#define DIRECTION_PIN D8   //Wemos pin connected to direction pin on stepper board
#define CLOSE_STOP_PIN D3

boolean findPositions = false;//set to true to find the correct gOpenGatePosition and gClosedGatePosition positions. This only needs to be done once

float maxStepperSpeed = 1000.0;
float stepperAcceleration = 600.0;

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(AccelStepper::DRIVER, STEP_PIN, DIRECTION_PIN);

//stepper.enableOutputs(); //can be used to turn the stepper driver on
//stepper.disableOutputs();//can be used to turn off the stepper driver

// ---------------- include the following functions to be called in the main code ---------------------
void openBlastGate(){
  //This gets called in the code when a button is pressed or a woodworking machine is turned on.
  //stepper.moveTo(gOpenGatePosition);//If gate is closed this will move to open. If open it will do nothing.
  //Serial.print statements below are just for debugging and can be comment out or removed if no longer needed.
    Serial.print("currentPosition is ");
    Serial.println( stepper.currentPosition() );
    Serial.println("Opening blast gate"); 
    
    stepper.enableOutputs();
    if(findPositions){ //If findPositions is set to true then each time the button is pressed it will move away from the stop switch. 
      // Take note of the position printed to the serial monitor and set gOpenGatePosition and gClosedGatePosition in the code above based on those readings
      // The -400 in the line may need to be changed for your stepper. It is two revolutions for most NEMA 17 steppers making full steps.
      stepper.runToNewPosition(stepper.currentPosition() -400);
    }else{
 stepper.moveTo(gOpenGatePosition);//gOpenGatePosition - stepper.currentPosition());
    }
 }
  void closeBlastGate(){  
  //This gets called when the gate needs to be closed. It is normally in response to a different gate being opened
  //stepper.moveTo(0);//move from the current position to the zero position
   //Serial.print statements below are just for debugging and can be comment out or removed if no longer needed.
    Serial.print("currentPosition is ");
    Serial.println( stepper.currentPosition() );
    Serial.println("Closing blast gate");
    stepper.enableOutputs();
    if(findPositions){
      stepper.runToNewPosition(stepper.currentPosition() -400);
    }else{
  stepper.moveTo(gClosedGatePosition);//-stepper.currentPosition());//move from the current position to the zero position
    }
 }

void gateSetup(){
  Serial.begin(115200);
  pinMode(CLOSE_STOP_PIN, INPUT_PULLUP);
  pinMode(ENABLE_PIN,OUTPUT);
  digitalWrite(ENABLE_PIN,LOW);
  stepper.setEnablePin(ENABLE_PIN);
  stepper.setMaxSpeed(maxStepperSpeed);
  stepper.setAcceleration(maxStepperSpeed);
  stepper.setPinsInverted(false,false,true);//just invert theenable pin
   //Serial.print statements below are just for debugging and can be comment out or removed if no longer needed.
  Serial.println("");
  Serial.println("Before running stepper to close position");
  Serial.print("Close stop pin is ");
  Serial.println(digitalRead(CLOSE_STOP_PIN));
  
  delay(1000);
  stepper.enableOutputs();
  while (digitalRead(CLOSE_STOP_PIN)==HIGH) {//stop switch is normally closed so it will read LOW until the magnet is close. 
    stepper.runToNewPosition(stepper.currentPosition() +400);
    delay(0);
    }
    Serial.println("Done moving stepper to stop switch");
    stepper.setCurrentPosition(0); //set the current position to zero and also sets speed to zero
    stepper.setMaxSpeed(maxStepperSpeed);
    stepper.setAcceleration(stepperAcceleration);
    closeBlastGate();
}
 void gateLoop(){
  stepper.run();
  if(!stepper.isRunning()){
    stepper.disableOutputs();
  }
  
}
