// Use this file if you are using this current sensor from Modern Device
// https://moderndevice.com/product/current-sensor/
// Cost is $16 each on 12/15/2020
// The link above has sample software and links discussing using this device with ESP8266 running 3.3V
// This device returns a differential value between the high and low of the AC current being monitored 
// unlike the ACS712 that returns a raw value and the difference needs to be calculated.
#define SENSOR_H "currentSensorModDevice.h"

#define SAMPLES 500   //Number of samples you want to take in total reading once every time through the loop
#define CHATTER 75    //Analog pin isn't stable so ignore average values below this level
#define Analog_Pin A0    //Use the only analog pin.

int gCurrentOn = 0; //0 is off, 1 is on, -1 is not done sampling
int gSampleCount =0;
unsigned long gTotal;


//------------ any functions needed ------------------
int readAcCurrentAsync() { //readAcCurrentAsync function checks the A0 measurement once. 
  // When SAMPLES number of times through this function it calculates the average reading

   int analogValue = analogRead(Analog_Pin); //We read a single value from the module
   gTotal =gTotal + analogValue; 
   gSampleCount++;
   
   if(gSampleCount >= SAMPLES){//We keep looping until we take all samples and at the end we will have a total to use to get an average.
     int averageValue = gTotal/SAMPLES;

     Serial.print("average sensor reading is ");
     Serial.println(averageValue);
        
     gSampleCount =0; // reset to zero so we start over again
     gTotal = 0;      // reset to zero so we start over again
     if (averageValue > CHATTER){
        //Serial.println("current is on");
        return 1;// tool seems to be on
      }else{
        //Serial.println("current is OFF");
        return 0; //tool seems to be off
      }
   }else{
      return -1;// more samples needed  
   }
}

void powerTurnedOn(){// This function will be called once, when the powerSensor is high after being low for a long time.
  String msg = gCloseOther;//message to other nodes
  mesh.sendBroadcast( msg );
  openBlastGate();
  gCurrentOn = true;

  Serial.println("current has turned on");
}
void powerTurnedOff(){ //don't do anything other than recording that the power is off.
  gCurrentOn = false;
}
void sensorSetup(){
    pinMode(Analog_Pin,INPUT);        //Define pin mode
}
 void sensorLoop(){
  int onOrOff = readAcCurrentAsync();  //read if the power is on to the woodworking tool
 
  if(gCurrentOn == 0 && onOrOff == 1){ //if just turned on
    powerTurnedOn();
    
  }else if(onOrOff == 0 && gCurrentOn ==1){ //if off just make sure gCurrentOn is set to 
    powerTurnedOff();
    Serial.println("current is on");
  }
}
