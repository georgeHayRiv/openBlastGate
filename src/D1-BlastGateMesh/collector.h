//TODO
// add code to check dust level, notify and turn off if full
// add OLED display of dust level


// Use these below when using a PNP transistor to turn an SSR on
//#define RELAY_PIN D6
//#define RELAY_ON LOW
//#define RELAY_OFF HIGH

// Use these below when using the Wemos Relay Shield
#define COLLECTOR //We use this to change the code in the main file so we can remotely turn the collector on or off based on the message received.
#define RELAY_PIN D1  //This is the default pin for the Wemos Relay Shield. Version 2.0.0 of the shield can be configured to use other pins. See below
#define RELAY_ON HIGH
#define RELAY_OFF LOW

// If using an SSR to directly turn the dust collector on D6 or any other pin not being used will work. The Wemos may not have enough 
// voltage, 3.3V to turn on the SSR or enough amperage to turn it on so to be safe have the 3.3 turn on a transistor wired to the 5V pin. See the 
// Fritzing diagram.
// If using the Wemos Relay shield version 2.0 to turn on a contactor the pin on the shield can be consigured to be D0, D1. D2, D5, D6. D7 or D8 The default is D1
// Version 1 of the Relay Shield was set at D1
// As of today, 11/12/2020 all the clone relay shields seem to be version 1 shield even if they are advertized as version 2.
// Here is the original shield, version 2 from the AliExpress
// LOLIN store https://www.aliexpress.com/item/32596395175.html

void gateSetup(){
  pinMode(RELAY_PIN, OUTPUT); 
}
void dustCollectorOff(){
  //This gets called in the code when a button is pressed and a message is sent to the other devices to close
  //In the case this is set up as a dust collector we want to turn the collector off since the shut off button on the collector was pushed.
  digitalWrite(RELAY_PIN,RELAY_OFF);//turn off dust collector
 }
 void openBlastGate(){//for compatability with buttons for gates
  dustCollectorOff();
 }
  void dustCollectorOn(){  
  digitalWrite(RELAY_PIN,RELAY_ON);//turn dust collector on
 }
void closeBlastGate(){//for compatability with buttons for gates
  dustCollectorOn();
 }

 void gateLoop(){
  //nothing to do in the loop for the dust collector
}
