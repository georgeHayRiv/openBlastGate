//woodworkers dust collector gate control using painlessMesh

#include "globals.h" // variables used by multiple .h files are defined here including mesh prefix, password and port. Additional variables may be defined in the various .h file.

// All options use painlessMesh to set up a WiFi network between all blast gates and the dust collector.
#include "painlessMesh.h"
Scheduler userScheduler; // to control your personal tasks and monitored within painlessMesh. The userScheduler is currently used by gateServo.. to turn off power to the servo after it has moved.
painlessMesh  mesh; //setting up an instance of painlessMesh



// uncomment only one of the following four include statements. These move the blast gates
//#include "gateServo.h"
//#include "gateServoRatchet.h"
//#include "gateRelay.h"        //use for pneumatic cylinder
//#include "gateStepper.h"      //use accelStepA4988.h instead
//#include "gateStepperA4988.h" //use accelStepA4988.h instead
#include "accelStepA4988.h"
//#include "collector.h"
//#include "gateNone.h"
//TODO #include "gateDcMotor.h" //use for a DC motor like a linear actuator which needs to run in both directions. No prototype for this yet.

// uncomment only one of the following include statements. These are the buttons or switches that trigger moving the blast gate (or turn on the dust collector)
//#include "button.h"
#include "toggleButton.h"
//#include "noButton.h"


// uncomment only one of the following include statements. These are the current sensors that trigger moving the blast gate and/or turn on the dust collector
//#include "currentSensorACS712.h"
#include "currentSensorModDevice.h"
//#include "noCurrentSensor.h"  //uncomment this include if you are not using one of the current sensors

void sendMessage() ; // Prototype so PlatformIO doesn't complain
void receivedCallback( uint32_t from, String &msg ) { // Needed for painless library
  #ifdef COLLECTOR
    if(msg == gVacOff){ // A remote button that was set up to turn the collect off has sent the msg to turn off the collector
      dustCollectorOff();
    }else{ // all other messages turn the collector on
      dustCollectorOn();
    }
  #else  //all gates do not define COLLECTOR so just send a message to the other gates to close since the sender is opening their gate.
  closeBlastGate();// We don't care what the messages says because we close our gate when any other is opened and sends us a message.
  #endif
}
 
void setup() {
  Serial.begin(115200); //only used for debugging
  //
   mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION  | MSG_TYPES | REMOTE ); // all types except GENERAL
 //  mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
 // mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
 
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT ); //iniatilize the mesh network
  mesh.onReceive(&receivedCallback); //any time a message is received from another blast gate we close this devices gate. That is done in the receivedCallback() function
  gateSetup();
  sensorSetup();
  buttonSetup();
}
void loop() {
  mesh.update(); // it will run the user scheduler as well as all operations need to make the WiFi mesh work
  sensorLoop();  // This will check any cureent sensor that might be part of this blast gate setup
  buttonLoop();  // This will check any button that might be part of this blast gate or dust collector setup
  gateLoop();    // This will check if anything needs to be done in the loop for this blast gate or dust collector setup
}
