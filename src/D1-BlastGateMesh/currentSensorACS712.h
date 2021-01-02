//This current sensor pretty cheap but does require that the AC line going to the woodworking tool be cut and each end attached to the terminals on this board.
//Boards are available in 5 amp, 20 amp and 30 amp versions. Unless a tool will draw more than 20 amps use the 20 amp version.

//TODO make sure the if statements in the loop don't trigger the powerTurnedOff function when just doing the next reading going through the loop.
//See how the code was changed in currentSensorModDevice.h

#define SAMPLES 300   //Number of samples you want to take everytime you loop
#define CHATTER 19    //Analog pin isn't stable so ignore differences in the max and min values below this level
#define ACS_Pin A0    //ACS712 data pin analog input

boolean gCurrentOn = false;
int gSampleCount =0;
int gMaxValue = 0;
int gMinValue = 1024;

//------------ any functions needed ------------------
boolean readAcCurrentAsync() { //readAcCurrentAsync function checks the A0 measurement once. 
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
        return true;// tool seems to be on
      }else{
        return false; //tool seems to be off
      }
   }else{
      return false;// more samples needed  
   }
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
}
 void sensorLoop(){
  boolean onOrOff = readAcCurrentAsync();  //read if the power is on to the woodworking tool
  if(gCurrentOn == false && onOrOff == true){ //if just turned on
    powerTurnedOn();
  }else if(onOrOff == false){ //if off just make sure gCurrentOn is set to 
    powerTurnedOff();
  }
}
