#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//add the includes for the GY-85, I2C, and create the object
#include "GY_85.h"
#include <Wire.h>
GY_85 GY85;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

//define the pins Blynk will poll for average, max, and minimum acceleration (in the x axis in this example
#define PIN_ACCEL V5
#define MAX_PIN V6
#define MIN_PIN V7

//set up the variables for the running average
const int numSamples = 100;
int counter = 0;
int runningArray [numSamples];

void setup()
{
  Wire.begin(D1,D2);
  delay(10);
  GY85.init();
  Serial.begin(9600);
  Blynk.begin(auth, "ssid", "pass");
}

int avgRun(){
  int sum = 0;
  for(int i=0;i<numSamples;i++){ //go thru all of the samples
    sum += runningArray[i];//add the samples to the sum of samples
  }
  sum = sum/numSamples;//divide the sum by the number of samples to get an average
  return sum;
}

int minRun(){
  //to calculate the minimum, take the first value in the array
  int nmin = runningArray[0];
   for(int i=0;i<numSamples;i++){
    //compare it to the other numbers in the array
    if(nmin>runningArray[i]){
      //and if it’s smaller, it’s the new minimum
      nmin = runningArray[i];
    }
  }
  return nmin;
}

int maxRun(){
  //to calculate the maximum, take the first value in the array
  int nmax = runningArray[0];
   for(int i=0;i<numSamples;i++){
    //compare it to the other numbers in the array
    if(nmax<runningArray[i]){
      //and if it’s larger, it’s the new maximum
      nmax = runningArray[i];
    }
  }
  return nmax;
}

void loop()
{
  Blynk.run();

  //check to make sure you loop
  if(counter > numSamples - 1){
    counter = 0;
    Serial.println(avgRun());
    }
  runningArray[counter] = GY85.accelerometer_x( GY85.readFromAccelerometer());
  counter ++;
   
}

