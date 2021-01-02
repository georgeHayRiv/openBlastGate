//Use this option when turning on a relay will open the gate.
// This will work for triggering a Solenoid Valve like this one.
// https://www.amazon.com/gp/product/B07SGCSGP8/ref=as_li_ss_tl?ie=UTF8&psc=1&linkCode=sl1&tag=robertcowandi-20&linkId=f66dcc44805cd6b79d53b7701601a177&language=en_US
// to activate a Pneumatic Air Cylinder like this one.
// https://www.amazon.com/Sydien-16x100mm-Pneumatic-Connector-MAL16x100/dp/B07DYPM7XB/ref=sr_1_13?dchild=1&keywords=16mm+Bore+100mm+Stroke+Air+Cylinder+Double+Action&qid=1607208422&s=industrial&sr=1-13
// as was used in this video
// https://www.youtube.com/watch?v=Sdo4Y9uW8y4


// Use these below when using a PNP transistor to turn an SSR on
//#define RELAY_PIN D6
//#define RELAY_ON LOW
//#define RELAY_OFF HIGH

// Use these below when using the Wemos Relay Shield
#define RELAY_PIN D1  //This is the default pin for the Wemos Relay Shield. Version 2.0.0 of the shield can be configured use other pins. See below
#define RELAY_ON HIGH
#define RELAY_OFF LOW

// If using an SSR to directly turn the Solenoid Valve or other device on D6 or any other pin not being used will work. The Wemos may not have enough 
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
void openBlastGate(){
  //This gets called in the code when a button is pressed and a message is sent to the other devices to close
  digitalWrite(RELAY_PIN,RELAY_ON);//open the blast gate
 }
  void closeBlastGate(){  
  digitalWrite(RELAY_PIN,RELAY_OFF);//close the blast gate
 }


 void gateLoop(){
  //nothing to do in the loop for this code
}
