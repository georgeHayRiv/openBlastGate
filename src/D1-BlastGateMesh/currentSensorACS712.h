//This current sensor pretty cheap but does require that the AC line going to the woodworking tool be cut and each end attached to the terminals on this board.
//Boards are available in 5 amp, 20 amp and 30 amp versions. Unless a tool will draw more than 20 amps use the 20 amp version.

//TODO make sure the if statements in the loop don't trigger the powerTurnedOff function when just doing the next reading going through the loop.
//See how the code was changed in currentSensorModDevice.h
#define SENSOR_H "currentSensorAC712.h"
#define USE_CURRENT_SENSOR
#define SAMPLES 100   //Number of samples you want to take everytime you loop
#define CHATTER 19    //Analog pin isn't stable so ignore differences in the max and min values below this level
#define ACS_Pin A0    //ACS712 data pin analog input
#define MILLIS_BETWEEN_SAMPLES 10 //10 works out to doing 100 reads in a second and Wifi still works

boolean gCurrentOn = false;
int gSampleCount =0;
int gMaxValue = 0;
int gMinValue = 1024;

//prototype functions
void readAcCurrentAsync();
void powerTurnedOn();
void powerTurnedOff();

//TaskScheduler is included by painlessMesh and tasks are checked in the loop() by painlessMesh
Task taskCheckSensor( MILLIS_BETWEEN_SAMPLES , TASK_FOREVER, &readAcCurrentAsync );//if we check too often it messes up the painlassMesh WiFi connection

//------------ any functions needed ------------------
void readAcCurrentAsync() { //readAcCurrentAsync function checks the A0 measurement once. 
  // When SAMPLES number of times through this function it calculates the difference between the high peak and low peak
  if( gSampleCount ==0){
      gMaxValue = 0;      //We first assume that our high peak is equal to 0 and low peak is 1024, yes inverted
      gMinValue = 1024;
  }
   int ACS_Value = analogRead(ACS_Pin); //We read a single value from the module
   gMaxValue =max(gMaxValue,ACS_Value); 
   gMinValue= min(gMinValue, ACS_Value);
   gSampleCount++;
   if(gSampleCount >= SAMPLES){//We keep looping until we take all samples and at the end we will have the high/low peaks values
     gSampleCount =0; // reset to zero so we start over again
     int diff= gMaxValue - gMinValue;
      if (diff > CHATTER){
        if(gCurrentOn == false){ //if just turned on
          powerTurnedOn();
          Serial.println("Power turned on");
        }//else don't do anything since we must have already signaled the tool is on
      }else{//tool seems to be off
        powerTurnedOff();//we can call this repeadily because it only sets gCurrentOn to false
        Serial.println("Power is off");
      }
   }//else we  still need more sample
}
void powerTurnedOn(){// This function will be called once, when the powerSensor is high after being low for a long time.
  String msg = gCloseOther;//message to other nodes
  mesh.sendBroadcast( msg );
  openBlastGate();
  gCurrentOn = true;
}
void powerTurnedOff(){ //don't do anything other than recording that the power is off.
  gCurrentOn = false;
}
void sensorSetup(){
    pinMode(ACS_Pin,INPUT);        //Define pin mode
    userScheduler.addTask( taskCheckSensor );
    taskCheckSensor.enable();
}
 void sensorLoop(){
//  boolean onOrOff = readAcCurrentAsync();  //read if the power is on to the woodworking tool
//  if(gCurrentOn == false && onOrOff == true){ //if just turned on
//    powerTurnedOn();
//  }else if(onOrOff == false){ //if off just make sure gCurrentOn is set to 
//    powerTurnedOff();
//  }
}
