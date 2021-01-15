// For use with the Wemos D1 OLED shield. This will not work with the OLED built into the d-duino
#define DISPLAY_H "displayOLED.h"

  #include <Wire.h>  // Include Wire if you're using I2C
  #include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library
  #define PIN_RESET 255  //
  #define DC_JUMPER 0  // I2C Addres: 0 - 0x3C, 1 - 0x3D
  MicroOLED oled(PIN_RESET, DC_JUMPER); 
  const int displayDelay = 5000; //millis to wait between display updates
  int gNumOfNodes = 0;

void newConnectionCallback(uint32_t nodeId) { //we want to know how many bodes there are currently connected. This returns 0 when none are found but two when the first is connected
  gNumOfNodes = mesh.getNodeList().size() + 1;
  Serial.print("New number of Nodes is ");
  Serial.println(gNumOfNodes);
}

void changedConnectionCallback() {//we want to know how many bodes there are currently connected. This returns 0 when none are found but two when the first is connected
  gNumOfNodes = mesh.getNodeList().size() + 1;
  Serial.print("Changed number of Nodes is ");
  Serial.println(gNumOfNodes);
}
  void updateOLED(){

  oled.clear(ALL); 
  oled.clear(PAGE);
  oled.setCursor(0,0); 
  oled.setFontType(0); 
  #ifdef COLLECTOR

// display if collector on or off
    oled.println("Collector");
    if(gGateStatus==GATE_OPEN){
      oled.println("is On");
    }else{
      oled.println("is Off");
    }
    //TODO
   // display sawdust level for collector
   oled.println("DustLevel");
   oled.println("is unknown");
  #else
  //TODO
  //display tool name or gate type
  oled.println("Gate is");
  if(gGateStatus==GATE_OPEN){
    oled.println("Open");
  }else{
    oled.println("Closed");
  }
  #ifdef USE_CURRENT_SENSOR
  
  oled.print("Tool ");
  if(gCurrentOn==true){
    oled.println("on");
  }else{
    oled.println("off");
  }
  #endif
  //TODO
  // if sensor is being used, display current sensor reading, tool on or off
  #endif
  oled.print(gNumOfNodes);
  oled.println(" Nodes");

// display current version 
  oled.print("Ver. ");
  oled.println(OBG_VERSION);
   oled.display();
  }

//TaskScheduler is included by painlessMesh and tasks are checked in the loop() by painlessMesh
Task taskUpdateOLED( displayDelay ,TASK_FOREVER, &updateOLED );

// Prototypes so PlatformIO doesn't complain
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback(); 



  
  void displaySetup(){
   Wire.begin();//this was removed in later versions of the SFE_MicroOLED.h library
    // These three lines of code are all you need to initialize the
    // OLED and print the splash screen.
    
    // Before you can start using the OLED, call begin() to init
    // all of the pins and configure the OLED.
    oled.begin();
    // clear(ALL) will clear out the OLED's graphic memory.
    // clear(PAGE) will clear the Arduino's display buffer.
    oled.clear(ALL);  // Clear the display's memory (gets rid of artifacts)
    // To actually draw anything on the display, you must call the
    // display() function. 
  oled.display(); 
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  userScheduler.addTask( taskUpdateOLED );
  taskUpdateOLED.enable();
}
void displayLoop(){
  //nothing to do here, painlessMesh checks the task scheduler for us.
}

//TODO
// add oledSetup to main code

// add painlessMesh code to track # of nodes connected
