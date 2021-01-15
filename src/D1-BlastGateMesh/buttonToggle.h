// This .h file is used when we want the button to both open the gate and close the gate. 
//When any gate is opened the dust collector will turn on and stay on.
// If this button is double clicked it will send a message and the dust collector will turn off
#define BUTTON_H "buttonToggle.h"
#include "OneButton.h"
#define BUTTON_PIN D4
OneButton button(BUTTON_PIN,HIGH, true);// Setup a new OneButton instance

void singleClick(){// This function will be called once, when the button is high after being low for a long time.
  Serial.println("Button pressed");
  if(gGateStatus==GATE_CLOSED){
    String msg = gCloseOther;//message to other nodes
    mesh.sendBroadcast( msg );
    openBlastGate(); 
  }else{
    closeBlastGate();
  }
  //gGateStatus  = !gGateStatus; //This needs to be set on the open and close gate functions because othere things can trigger this
}
void doubleClick(){// This function will be called when a button has been double clicked
  Serial.println("double click detected, turn off dust collector");
  String msg = gVacOff;//message to all nodes. The dust collector checks and turns itself off when received. All other gates take any message sent as a signal to close their gate.
  mesh.sendBroadcast( msg ); // since we don't know which device on the mesh network is the dust collect or shop vac we are just sending this out to all nodes.
}
void buttonSetup(){
  button.attachClick(singleClick); //looking for a low read to signal gate opening
  button.attachDoubleClick(doubleClick); //looking for a low read to signal gate opening
  // set 80 msec. debouncing time. Default is 50 msec.
  button.setDebounceTicks(80);
}
 void buttonLoop(){
  button.tick(); //This checks the state of the button.
}
