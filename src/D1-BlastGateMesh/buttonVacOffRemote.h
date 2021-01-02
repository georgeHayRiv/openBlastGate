
//TODO remove this file. The double click code has been added to the button.h and toggleButton.h files


// This include file will only work if either the button.h or toggleButton.h file have been included in the sketch

// This header file just adds a response to a double click that is designed to tell the dust collector to turn off from a remote device.
// It is not designed to be included without first including button.h or toggleButton.h
// When uploading code to a dust collector this file would not make any sense. The toggleButton.h file alone will turn the collector on or off.

//#include "OneButton.h" //already included in the button.h or toggleButton.h file
//#define BUTTON_PIN D4 //already defined in the button.h or toggleButton.h file
//OneButton button(BUTTON_PIN,LOW, true);// Setup a new OneButton instance, This is already done in the button.h or toggleButton.h file

void doubleClick(){// This function will be called when a button has been double clicked
  Serial.println("double click detected, turn off dust collector");
  String msg = gVacOff;//message to all nodes. The dust collector checks and turns itself off when received. All other gates take any message sent as a signal to close their gate.
  mesh.sendBroadcast( msg ); // since we don't know which device on the mesh network is the dust collect or shop vac we are just sending this out to all nodes.
}
void buttonSetup(){
  button.attachDoubleClick(doubleClick); //looking for a low read to signal gate opening
}
 void buttonLoop(){
  //button.tick(); //This checks the state of the button but is already being done in the button.h or toggleButton.h file
}
