//woodworkers dust collector gate control using painlessMesh

#define OBG_VERSION "0.01"  ///1/13/2021
#include "globals.h" // variables used by multiple .h files are defined here including mesh prefix, password and port. Additional variables may be defined in the various .h file.

// All options use painlessMesh to set up a WiFi network between all blast gates and the dust collector.
#include "painlessMesh.h"
Scheduler userScheduler; // to control your personal tasks and monitored within painlessMesh. The userScheduler is currently used by gateServo.. to turn off power to the servo after it has moved.
painlessMesh  mesh; //setting up an instance of painlessMesh



// uncomment only one of the following include statements. These move the blast gates
//#include "gateServo.h"
//#include "gateServoRatchet.h"
//#include "gateRelay.h"        //use for pneumatic cylinder
//#include "gateStepper.h"      //use accelStepA4988.h instead
//#include "gateStepperA4988.h" //use accelStepA4988.h instead
#include "accelStepA4988.h"
//#include "collector.h"
//#include "gateNone.h"
//TODO #include "gateDcMotor.h" //use for a DC motor like a linear actuator which needs to run in both directions. No prototype for this yet.
//TODO #include "gateDcMotorUni.h" //use for a DC motor like a windshield wiper motor or circlular gate that only runs in one direction. No workable prototype for this yet.

// uncomment only one of the following include statements. These are the buttons or switches that trigger moving the blast gate (or turn on the dust collector)
//#include "button.h"
#include "buttonToggle.h"
//#include "buttonNone.h"


// uncomment only one of the following include statements. These are the current sensors that trigger moving the blast gate and/or turn on the dust collector
//#include "currentSensorACS712.h"
//#include "currentSensorModDevice.h"
#include "currentSensorNone.h"  //uncomment this include if you are not using one of the current sensors

// uncomment only one of the following include statements. These are for adding a display, OLED or LCD to a particular gate or collector
//#include "displayOLED.h"
#include "displayNone.h"
//TODO set up other display devices if needed.

// Prototypes so PlatformIO doesn't complain
void sendMessage() ; 

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
  //---------- mesh network stuff to set up. -------------------------
  // mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION  | MSG_TYPES | REMOTE ); // all types except GENERAL
 //  mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
 
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT ); //iniatilize the mesh network. These variables are set in globals.h file
  mesh.onReceive(&receivedCallback); //any time a message is received from another blast gate we close this devices gate. That is done in the receivedCallback() function
  //the displayOLED adds mesh.onNewConnection(&newConnectionCallback); and mesh.onChangedConnections(&changedConnectionCallback); if it is included to track the number of nodes connected
  //---------- end of mesh network stuff to set up. -------------------------
  gateSetup();
  sensorSetup();
  buttonSetup();
  displaySetup();
  //-------------- at startup this will list the include files that have been loaded on the Wemos D1 for this node/blast gate/tool
  Serial.print("openBlastGate Version ");
  Serial.println(OBG_VERSION);
  Serial.print("Included gate header file ");
  Serial.println(GATE_H);
  Serial.print("Included button header file ");
  Serial.println(BUTTON_H);
  Serial.print("Included sensor header file ");
  Serial.println(SENSOR_H);
  Serial.print("Included display header file ");
  Serial.println(DISPLAY_H);
}
void loop() {
  mesh.update(); // it will run the user scheduler as well as all operations need to make the WiFi mesh work
  sensorLoop();  // This will check any cureent sensor that might be part of this blast gate setup
  buttonLoop();  // This will check any button that might be part of this blast gate or dust collector setup
  gateLoop();    // This will check if anything needs to be done in the loop for this blast gate or dust collector setup
  displayLoop(); // If needed this will check to see if the display needs to be updated. Normally userScheduler does this in the painlessMesh mesh.update(); above.
}
