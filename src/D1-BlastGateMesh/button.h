#include "OneButton.h"
#define BUTTON_PIN D4
OneButton button(BUTTON_PIN,LOW, true);// Setup a new OneButton instance

void singleClick(){// This function will be called once, when the button is clicked
  Serial.println(button clicked");
  String msg = gCloseOther;//message to other nodes
  mesh.sendBroadcast( msg );
  openBlastGate();
}
void doubleClick(){// This function will be called when a button has been double clicked
  Serial.println("double click detected, turn off dust collector");
  String msg = gVacOff;//message to all nodes. The dust collector checks and turns itself off when received. All other gates take any message sent as a signal to close their gate.
  mesh.sendBroadcast( msg ); // since we don't know which device on the mesh network is the dust collect or shop vac we are just sending this out to all nodes.
}
void buttonSetup(){
  button.attachClick(singleClick); //setup to look for a button click
  button.attachDoubleClick(doubleClick); //setup to look for a double click of the button
 void buttonLoop(){
  button.tick(); //This checks the state of the button.
}
