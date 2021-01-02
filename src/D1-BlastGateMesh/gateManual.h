//A template to use for a blast gate that gets opened and closed manually and has stop switches to indicate if it is open or closed

//TODO add oneButton library.
//TODO this might be made to work with a single stop switch for just when the gate has been opened,

//define the pins used for gate open and gate closed
#define OPEN_STOP_PIN D6
#define CLOSED_STOP_PIN D7
// add any global variables
// do any initialization
// define any local functions used by this file

//TODO add oneButton code to set up buttons

// ---------------- include the following functions to be called in the main code ---------------------
void openBlastGate(){
  //This gets called in the code when a button is pressed or a woodworking machine is turned on.
  
 }
  void closeBlastGate(){  
  //This gets called when the gate needs to be closed. It is normally in response to a different gate being opened
  
 }

void gateSetup(){
  // add any code that needs to run in the setup.
  //TODO set gGateStatus based on which stop is active
}
 void gateLoop(){
  //if any action needs to be done in the loop add it here, if not just leave it blank.
  //TODO read button status
  
}
